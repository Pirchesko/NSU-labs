#!/bin/bash
K=$4
N=$3
labc=$1
labexe=$2
#echo $labc
#echo $labexe 
#echo $N 
#echo

gcc -o $labexe $labc -Ofast -g -march=native -pthread
for (( i=1; i <= $N; i++ ))
do
./$labexe $K
done
#echo