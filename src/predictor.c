#include <stdio.h>
#include <math.h>
#include "predictor.h"

#define DEBUG 0
#define N 12 // preceptron history length
#define SG 0
#define WG 1
#define WL 2
#define SL 3

const char *studentName = "Stevie Huang";
const char *studentID   = "A59019267";
const char *email       = "weh019@ucsd.edu";

// Handy Global for use in output routines
const char *bpName[4] = {"Static", "Gshare", "Tournament", "Custom" };

int ghistoryBits; // Number of bits used for Global History
int lhistoryBits; // Number of bits used for Local History
int pcIndexBits;  // Number of bits used for PC index
int bpType;       // Branch Prediction Type
int verbose;


//Gshare
uint32_t GHR; // global history register
uint8_t *gsharePatternTable;

//Tournament
uint32_t globalHistory;
uint8_t *globalPatternTable;
uint32_t *localHistroyTable;
uint8_t *localPatternTable;
uint8_t *selectionTable;

//Perceptron
typedef int* preceptron;
preceptron* ptable;
int* historyTable;
uint32_t pGlobalHistory;


uint32_t getMask(int length) {
    return ~(~0 << length);
}

uint32_t pow2(int num) {
    uint32_t rst = 1;
    for (int i = 0; i < num; ++ i) {
        rst = rst << 1;
    }
    return rst;
}

void init_gshare() {
    GHR = NOTTAKEN;
    gsharePatternTable = (uint8_t*) malloc(pow2(ghistoryBits) * sizeof(uint8_t));
    for (uint32_t i = 0; i < pow2(ghistoryBits); ++ i) {
        gsharePatternTable[i] = NOTTAKEN;
    }
}

uint8_t make_prediction_gshare(uint32_t pc) {
    uint32_t mask = getMask(ghistoryBits);
    uint32_t index = (GHR & mask) ^ (pc & mask);

    if (gsharePatternTable[index] == SN || gsharePatternTable[index] == WN) {
        return NOTTAKEN;
    }
    return TAKEN;
}

void incrementPattern(uint8_t *pattern) {
    if (*pattern == SN || *pattern == WN || *pattern == WT) {
        (*pattern) += 1;
    }
}

void decrementPattern(uint8_t *pattern) {
    if (*pattern == WN || *pattern == WT || *pattern == ST) {
        (*pattern) -= 1;
    }
}

void train_predictor_gshare(uint32_t pc, uint8_t outcome) {
    uint32_t mask = getMask(ghistoryBits);
    uint32_t index = (GHR & mask) ^ (pc & mask);

    if (outcome == TAKEN) {
        if (DEBUG) {
            printf("pattern before training: %d\n", gsharePatternTable[index]);
            incrementPattern(&gsharePatternTable[index]);
            printf("pattern after training: %d\n", gsharePatternTable[index]);
        } else {
            incrementPattern(&gsharePatternTable[index]);
        }
    } else {
        if (DEBUG) {
            printf("pattern before training: %d\n", gsharePatternTable[index]);
            decrementPattern(&gsharePatternTable[index]);
            printf("pattern after training: %d\n", gsharePatternTable[index]);
        } else {
            decrementPattern(&gsharePatternTable[index]);
        }
    }

    if (DEBUG) {
        printf("GHR before training: %d\n", GHR);
    }
    GHR = ((GHR << 1) | outcome) & mask;
    if (DEBUG) {
        printf("GHR after training: %d\n", GHR);
    }
}

void init_tournament() {
    globalHistory = NOTTAKEN;
    globalPatternTable = (uint8_t*) malloc(pow2(ghistoryBits) * sizeof(uint8_t));
    for (uint32_t i = 0; i < pow2(ghistoryBits); ++ i) {
        globalPatternTable[i] = SN;
    }

    localHistroyTable = (uint32_t*) malloc(pow2(pcIndexBits) * sizeof(uint32_t));
    for (uint32_t i = 0; i < pow2(pcIndexBits); ++ i) {
        localHistroyTable[i] = (uint32_t)0;
    }
    localPatternTable = (uint8_t*) malloc(pow2(lhistoryBits) * sizeof(uint8_t));
    for (uint32_t i = 0; i < pow2(lhistoryBits); ++ i) {
        localPatternTable[i] = SN;
    }
    selectionTable = (uint8_t*) malloc(pow2(ghistoryBits) * sizeof(uint8_t));
    for (uint32_t i = 0; i < pow2(ghistoryBits); ++ i) {
        selectionTable[i] = SG;
    }
}

uint8_t make_prediction_global(uint32_t pc) {
    if (globalPatternTable[globalHistory] == SN || globalPatternTable[globalHistory] == WN) {
        return NOTTAKEN;
    }
    return TAKEN;
}

void train_predictor_global(uint32_t pc, uint8_t outcome) {
    uint32_t mask = getMask(ghistoryBits);
    if (outcome == TAKEN) {
        incrementPattern(&globalPatternTable[globalHistory]);
    } else {
        decrementPattern(&globalPatternTable[globalHistory]);
    }
    globalHistory = ((globalHistory << 1) | outcome) & mask;
}

uint8_t make_prediction_local(uint32_t pc) {
    uint32_t mask = getMask(pcIndexBits);
    uint32_t pcIndex = pc & mask;
    uint32_t history = localHistroyTable[pcIndex];
    uint8_t tounamentPrediction = localPatternTable[history];
    if (tounamentPrediction == SN || tounamentPrediction == WN) {
        return NOTTAKEN;
    }
    return TAKEN;
}

void train_predictor_local(uint32_t pc, uint8_t outcome) {
    uint32_t mask = getMask(pcIndexBits);
    uint32_t pcIndex = pc & mask;
    uint32_t historyIndex = localHistroyTable[pcIndex];
    if (outcome == TAKEN) {
        incrementPattern(&localPatternTable[historyIndex]);
    } else {
        decrementPattern(&localPatternTable[historyIndex]);
    }
    uint32_t historyMask = getMask(lhistoryBits);
    localHistroyTable[pcIndex] = ((localHistroyTable[pcIndex] << 1) | outcome) & historyMask;
}

uint8_t make_prediction_tournament(uint32_t pc) {
    uint8_t globalPrediction = make_prediction_global(pc);
    if (selectionTable[globalHistory] == SG || selectionTable[globalHistory] == WG) {
        return globalPrediction;
    }
    return make_prediction_local(pc);
}

void train_predictor_tournament(uint32_t pc, uint8_t outcome) {
    uint8_t globalPrediction = make_prediction_global(pc);
    uint8_t localPrediction = make_prediction_local(pc);
    if (globalPrediction != localPrediction) {
        if (globalPrediction == outcome) {
            decrementPattern(&selectionTable[globalHistory]);
        } else {
            incrementPattern(&selectionTable[globalHistory]);
        }
    }
    train_predictor_global(pc, outcome);
    train_predictor_local(pc, outcome);
}

void init_preceptron() {
    ptable = (preceptron*) malloc(pow2(N) * sizeof(preceptron));
    for (int i = 0; i < pow2(N); i += 1) {
        ptable[i] = (preceptron) malloc((N + 1) * sizeof(int));
        for (int j = 0; j <= N; j += 1) {
            ptable[i][j] = 0;
        }
    }
    pGlobalHistory = NOTTAKEN;
    historyTable = malloc((N + 1) * sizeof(int));
    historyTable[0] = 1;
    for (int i = 1; i <=N; i += 1) {
        historyTable[i] = -1;
    }
}

uint8_t getPrediction(int rst) {
    if (rst <= 0) return NOTTAKEN;
    return TAKEN;
}

uint8_t make_prediction_preceptron(uint32_t pc) {
    uint32_t mask = getMask(N);
    uint32_t index = (pc & mask) ^ (pGlobalHistory & mask);
    preceptron p = ptable[index];
    int rst = 0;
    for (int i = 0; i <= N; i += 1) {
        rst += (p[i] * historyTable[i]);
    }
    //printf("rst: %d\n", rst);
    return getPrediction(rst);
}

void train_predictor_preceptron(uint32_t pc, uint8_t outcome) {
    int t = outcome;
    if (outcome == 0) {
        t = -1;
    }
    uint32_t mask = getMask(N);
    uint32_t index = (pc & mask) ^ (pGlobalHistory & mask);
    preceptron p = ptable[index];
    int rst = 0;
    for (int i = 0; i <= N; i += 1) {
        rst += (p[i] * historyTable[i]);
    }
    if ((rst < 0) != (t < 0) || abs(rst) <= 40) {
        for (int i = 0; i <= N; i += 1) {
            p[i] += t * historyTable[i];
        }
    }
    for (int i = 1; i < N; i += 1) {
        historyTable[i] = historyTable[i + 1];
    }
    historyTable[N] = t;
    pGlobalHistory = ((pGlobalHistory << 1) | outcome) & mask;
}

void init_custom() {
    GHR = NOTTAKEN;
    gsharePatternTable = (uint8_t*) malloc(pow2(ghistoryBits) * sizeof(uint8_t));
    for (uint32_t i = 0; i < pow2(ghistoryBits); ++ i) {
        gsharePatternTable[i] = NOTTAKEN;
    }

    uint32_t length = pow2(pcIndexBits);
    localHistroyTable = (uint32_t*) malloc(length * sizeof(uint32_t));
    localPatternTable = (uint8_t*) malloc(pow2(lhistoryBits) * sizeof(uint8_t));
    selectionTable = (uint8_t*) malloc(length * sizeof(uint8_t));
    for (uint32_t i = 0; i < length; ++ i) {
        localHistroyTable[i] = (uint32_t)0;
        selectionTable[i] = SG;
    }
    for (uint32_t i = 0; i < pow2(lhistoryBits); ++ i) {
        localPatternTable[i] = SN;
    }
}

uint8_t make_prediction_custom(uint32_t pc) {
    uint8_t gsharePrediction = make_prediction_gshare(pc);
    uint32_t mask = getMask(pcIndexBits);
    uint32_t pcIndex = pc & mask;
    if (selectionTable[pcIndex] == SG || selectionTable[pcIndex] == WG) {
        return gsharePrediction;
    }
    return make_prediction_local(pc);
}

void train_predictor_custom(uint32_t pc, uint8_t outcome) {
    uint8_t gsharePrediction = make_prediction_gshare(pc);
    uint8_t localPrediction = make_prediction_local(pc);
    uint32_t mask = getMask(pcIndexBits);
    uint32_t pcIndex = pc & mask;
    if (gsharePrediction != localPrediction) {
        if (gsharePrediction == outcome) {
            decrementPattern(&selectionTable[pcIndex]);
        } else {
            incrementPattern(&selectionTable[pcIndex]);
        }
    }
    train_predictor_gshare(pc, outcome);

    uint32_t historyIndex = localHistroyTable[pcIndex];
    if (outcome == TAKEN) {
        incrementPattern(&localPatternTable[historyIndex]);
    } else {
        decrementPattern(&localPatternTable[historyIndex]);
    }
    uint32_t historyMask = getMask(lhistoryBits);
    localHistroyTable[pcIndex] = ((localHistroyTable[pcIndex] << 1) | outcome) & historyMask;
}

void init_predictor() {
    switch (bpType)
    {
    case GSHARE:
        init_gshare();
        break;
    case TOURNAMENT:
        init_tournament();
        break;
    case CUSTOM:
        init_custom();
        break;
    case PERCEPTRON:
        init_preceptron();
        break;
    default:
        break;
    }
}

// Make a prediction for conditional branch instruction at PC 'pc'
// Returning TAKEN indicates a prediction of taken; returning NOTTAKEN
// indicates a prediction of not taken
uint8_t make_prediction(uint32_t pc) {
  switch (bpType) {
    case STATIC:
      return TAKEN;
    case GSHARE:
        return make_prediction_gshare(pc);
    case TOURNAMENT:
        return make_prediction_tournament(pc);
    case CUSTOM:
        return make_prediction_custom(pc);
    case PERCEPTRON:
        return make_prediction_preceptron(pc);
    default:
      break;
  }
  return NOTTAKEN;
}

void train_predictor(uint32_t pc, uint8_t outcome) {
  switch (bpType) {
    case GSHARE:
        train_predictor_gshare(pc, outcome);
        break;
    case TOURNAMENT:
        train_predictor_tournament(pc, outcome);
        break;
    case CUSTOM:
        train_predictor_custom(pc, outcome);
        break;
    case PERCEPTRON:
        train_predictor_preceptron(pc, outcome);
        break;
    default:
      break;
  }
}
