#! /bin/bash
echo -e "Enter Predictor Type:\n1.Gshare\n2.Tournament\n3.Custom1\n4.Custom2\n5.Perceptron\n6.Run all"
read type
if [ $type == "1" ]
then
echo "running gshare predictor..."
bunzip2 -kc ../traces/fp_1.bz2 | ./predictor --gshare:13
bunzip2 -kc ../traces/fp_2.bz2 | ./predictor --gshare:13
bunzip2 -kc ../traces/int_1.bz2 | ./predictor --gshare:13
bunzip2 -kc ../traces/int_2.bz2 | ./predictor --gshare:13
bunzip2 -kc ../traces/mm_1.bz2 | ./predictor --gshare:13
bunzip2 -kc ../traces/mm_2.bz2 | ./predictor --gshare:13
elif [ $type == "2" ]
then
echo "running tournament predictor..."
bunzip2 -kc ../traces/fp_1.bz2 | ./predictor --tournament:9:10:10
bunzip2 -kc ../traces/fp_2.bz2 | ./predictor --tournament:9:10:10
bunzip2 -kc ../traces/int_1.bz2 | ./predictor --tournament:9:10:10
bunzip2 -kc ../traces/int_2.bz2 | ./predictor --tournament:9:10:10
bunzip2 -kc ../traces/mm_1.bz2 | ./predictor --tournament:9:10:10
bunzip2 -kc ../traces/mm_2.bz2 | ./predictor --tournament:9:10:10
elif [ $type == "3" ]
then
echo "running custom predictor..."
bunzip2 -kc ../traces/fp_1.bz2 | ./predictor --custom:9:10:10
bunzip2 -kc ../traces/fp_2.bz2 | ./predictor --custom:9:10:10
bunzip2 -kc ../traces/int_1.bz2 | ./predictor --custom:9:10:10
bunzip2 -kc ../traces/int_2.bz2 | ./predictor --custom:9:10:10
bunzip2 -kc ../traces/mm_1.bz2 | ./predictor --custom:9:10:10
bunzip2 -kc ../traces/mm_2.bz2 | ./predictor --custom:9:10:10
elif [ $type == "4" ]
then
echo "running custom predictor..."
bunzip2 -kc ../traces/fp_1.bz2 | ./predictor --custom:13:13:10
bunzip2 -kc ../traces/fp_2.bz2 | ./predictor --custom:13:13:10
bunzip2 -kc ../traces/int_1.bz2 | ./predictor --custom:13:13:10
bunzip2 -kc ../traces/int_2.bz2 | ./predictor --custom:13:13:10
bunzip2 -kc ../traces/mm_1.bz2 | ./predictor --custom:13:13:10
bunzip2 -kc ../traces/mm_2.bz2 | ./predictor --custom:13:13:10
elif [ $type == "5" ]
then
echo "running preceptron predictor..."
bunzip2 -kc ../traces/fp_1.bz2 | ./predictor --preceptron
bunzip2 -kc ../traces/fp_2.bz2 | ./predictor --preceptron
bunzip2 -kc ../traces/int_1.bz2 | ./predictor --preceptron
bunzip2 -kc ../traces/int_2.bz2 | ./predictor --preceptron
bunzip2 -kc ../traces/mm_1.bz2 | ./predictor --preceptron
bunzip2 -kc ../traces/mm_2.bz2 | ./predictor --preceptron
#bunzip2 -kc ../traces/test.bz2 | ./predictor --preceptron
elif [ $type == "6" ]
then 
echo "running gshare predictor..."
bunzip2 -kc ../traces/fp_1.bz2 | ./predictor --gshare:13
bunzip2 -kc ../traces/fp_2.bz2 | ./predictor --gshare:13
bunzip2 -kc ../traces/int_1.bz2 | ./predictor --gshare:13
bunzip2 -kc ../traces/int_2.bz2 | ./predictor --gshare:13
bunzip2 -kc ../traces/mm_1.bz2 | ./predictor --gshare:13
bunzip2 -kc ../traces/mm_2.bz2 | ./predictor --gshare:13
echo "running tournament predictor..."
bunzip2 -kc ../traces/fp_1.bz2 | ./predictor --tournament:9:10:10
bunzip2 -kc ../traces/fp_2.bz2 | ./predictor --tournament:9:10:10
bunzip2 -kc ../traces/int_1.bz2 | ./predictor --tournament:9:10:10
bunzip2 -kc ../traces/int_2.bz2 | ./predictor --tournament:9:10:10
bunzip2 -kc ../traces/mm_1.bz2 | ./predictor --tournament:9:10:10
bunzip2 -kc ../traces/mm_2.bz2 | ./predictor --tournament:9:10:10
echo "running custom predictor..."
bunzip2 -kc ../traces/fp_1.bz2 | ./predictor --custom:13:13:10
bunzip2 -kc ../traces/fp_2.bz2 | ./predictor --custom:13:13:10
bunzip2 -kc ../traces/int_1.bz2 | ./predictor --custom:13:13:10
bunzip2 -kc ../traces/int_2.bz2 | ./predictor --custom:13:13:10
bunzip2 -kc ../traces/mm_1.bz2 | ./predictor --custom:13:13:10
bunzip2 -kc ../traces/mm_2.bz2 | ./predictor --custom:13:13:10
else
echo "invalid option, exiting..."
fi