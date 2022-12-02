#include <stdio.h>
#include "predictor.h"
#define DEBUG 0
const char *studentName = "Wentao Huang";
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
uint32_t *gsharePatternTable;



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
    if (DEBUG) {
        printf("Start of init_gshare\n");
    }
    GHR = NOTTAKEN;
    gsharePatternTable = (uint32_t*) malloc(pow2(ghistoryBits) * sizeof(uint32_t));
    for (uint32_t i = 0; i < pow2(ghistoryBits); ++ i) {
        gsharePatternTable[i] = NOTTAKEN;
    }
}

uint8_t make_prediction_gshare(uint32_t pc) {
    if (DEBUG) {
        printf("Start of prediction_gshare\n");
    }
    uint32_t mask = getMask(ghistoryBits);
    uint32_t index = (GHR & mask) ^ (pc & mask);

    if (gsharePatternTable[index] == SN || gsharePatternTable[index] == WN) {
        return NOTTAKEN;
    }
    return TAKEN;
}

void incrementPattern(uint32_t *pattern) {
    if (*pattern == SN || *pattern == WN || *pattern == WT) {
        (*pattern) += 1;
    }
}

void decrementPattern(uint32_t *pattern) {
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

}

uint8_t make_prediction_tournament(uint32_t pc) {
    return NOTTAKEN;
}

void train_predictor_tournament(uint32_t pc, uint8_t outcome) {

}

void init_predictor() {
    switch (bpType)
    {
    case GSHARE:
        printf("Using Gshare predictor...\n");
        init_gshare();
        break;
    case TOURNAMENT:
        printf("Using Tournament predictor...\n");
        init_tournament();
        break;
    default:
        break;
    }
}

// Make a prediction for conditional branch instruction at PC 'pc'
// Returning TAKEN indicates a prediction of taken; returning NOTTAKEN
// indicates a prediction of not taken
//
uint8_t make_prediction(uint32_t pc) {
  switch (bpType) {
    case STATIC:
      return TAKEN;
    case GSHARE:
        return make_prediction_gshare(pc);
    case TOURNAMENT:
        return make_prediction_tournament(pc);
    case CUSTOM:
        return NOTTAKEN;
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
        break;
    default:
      break;
  }
}
