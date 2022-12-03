#! /bin/bash
echo -e "Enter Predictor Type:\n1. Static\n2.Gshare\n3.Tournament\n4.Custom\n"
read type
if [ $type == "1" ]
then
echo "running static predictor..."
bunzip2 -kc ../traces/fp_1.bz2 | ./predictor --static
bunzip2 -kc ../traces/fp_2.bz2 | ./predictor --static
bunzip2 -kc ../traces/int_1.bz2 | ./predictor --static
bunzip2 -kc ../traces/int_2.bz2 | ./predictor --static
bunzip2 -kc ../traces/mm_1.bz2 | ./predictor --static
bunzip2 -kc ../traces/mm_2.bz2 | ./predictor --static
elif [ $type == "2" ]
then
echo "running gshare predictor..."
bunzip2 -kc ../traces/fp_1.bz2 | ./predictor --gshare:13
bunzip2 -kc ../traces/fp_2.bz2 | ./predictor --gshare:13
bunzip2 -kc ../traces/int_1.bz2 | ./predictor --gshare:13
bunzip2 -kc ../traces/int_2.bz2 | ./predictor --gshare:13
bunzip2 -kc ../traces/mm_1.bz2 | ./predictor --gshare:13
bunzip2 -kc ../traces/mm_2.bz2 | ./predictor --gshare:13
elif [ $type == "3" ]
then
echo "running tournament predictor..."
bunzip2 -kc ../traces/fp_1.bz2 | ./predictor --tournament:9:10:10
bunzip2 -kc ../traces/fp_2.bz2 | ./predictor --tournament:9:10:10
bunzip2 -kc ../traces/int_1.bz2 | ./predictor --tournament:9:10:10
bunzip2 -kc ../traces/int_2.bz2 | ./predictor --tournament:9:10:10
bunzip2 -kc ../traces/mm_1.bz2 | ./predictor --tournament:9:10:10
bunzip2 -kc ../traces/mm_2.bz2 | ./predictor --tournament:9:10:10
elif [ $type == "4" ]
then
echo "running custom predictor..."
bunzip2 -kc ../traces/fp_1.bz2 | ./predictor --custom:12:12:10
bunzip2 -kc ../traces/fp_2.bz2 | ./predictor --custom:12:12:10
bunzip2 -kc ../traces/int_1.bz2 | ./predictor --custom:12:12:10
bunzip2 -kc ../traces/int_2.bz2 | ./predictor --custom:12:12:10
bunzip2 -kc ../traces/mm_1.bz2 | ./predictor --custom:12:12:10
bunzip2 -kc ../traces/mm_2.bz2 | ./predictor --custom:12:12:10
else
echo "invalid option, exiting..."
fi