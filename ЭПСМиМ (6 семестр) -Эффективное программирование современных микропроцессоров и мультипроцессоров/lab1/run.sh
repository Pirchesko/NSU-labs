#!/bin/bash
N=$3
labc=$1
labexe=$2
echo $labc
echo $labexe 
echo $N 
echo

echo "compiling: -O0"
gcc -o $labexe $labc -O0 -g
for (( i=1; i <= $N; i++ ))
do
./$labexe
done
echo

echo "compiling: -O1"
gcc -o $labexe $labc -O1 -g
for (( i=1; i <= $N; i++ ))
do
./$labexe
done
echo

echo "compiling: -O2"
gcc -o $labexe $labc -O2 -g
for (( i=1; i <= $N; i++ ))
do
./$labexe
done
echo

echo "compiling: -O3"
gcc -o $labexe $labc -O3 -g
for (( i=1; i <= $N; i++ ))
do
./$labexe
done
echo

echo "compiling: -Ofast"
gcc -o $labexe $labc -Ofast -g
for (( i=1; i <= $N; i++ ))
do
./$labexe
done
echo

echo "compiling: -Og"
gcc -o $labexe $labc -Og -g
for (( i=1; i <= $N; i++ ))
do
./$labexe
done
echo

echo "compiling: -O0 -march=native"
gcc -o $labexe $labc -O0 -g -march=native
for (( i=1; i <= $N; i++ ))
do
./$labexe
done
echo

echo "compiling: -O1 -march=native"
gcc -o $labexe $labc -O1 -g -march=native
for (( i=1; i <= $N; i++ ))
do
./$labexe
done
echo

echo "compiling: -O2 -march=native"
gcc -o $labexe $labc -O2 -g -march=native
for (( i=1; i <= $N; i++ ))
do
./$labexe
done
echo

echo "compiling: -O3 -march=native"
gcc -o $labexe $labc -O3 -g -march=native
for (( i=1; i <= $N; i++ ))
do
./$labexe
done
echo

echo "compiling: -Ofast -march=native"
gcc -o $labexe $labc -Ofast -g -march=native
for (( i=1; i <= $N; i++ ))
do
./$labexe
done
echo

echo "compiling: -Og -march=native"
gcc -o $labexe $labc -Og -g -march=native
for (( i=1; i <= $N; i++ ))
do
./$labexe
done
echo