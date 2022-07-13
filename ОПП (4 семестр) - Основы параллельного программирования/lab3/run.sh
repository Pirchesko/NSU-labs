#!/bin/bash

#PBS -l walltime=00:30:00
#PBS -l select=2:ncpus=8:mpiprocs=8:mem=2000m,place=free
#PBS -m n
cd $PBS_O_WORKDIR
MPI_NP=$(wc -l $PBS_NODEFILE | awk '{ print $1 }')
echo "Number of MPI process: $MPI_NP"
echo 'File $PBS_NODEFILE:'
cat $PBS_NODEFILE
echo

icc test.c -std=gnu99 -o test.exe
mpiicc lab3.c -std=gnu99 -o lab3.exe

echo "Without mpi: "
./test.exe

for (( i = 1; i <= 4; i++ ))
do
	for (( j = 1; j <= 4; j++ ))
	do
		echo "  "
		n=$(( J * i))
		echo "p1 = $i;     p2 = $j;     p1 * p2 = $n "
		mpirun -np $n ./lab3.exe $i $j
	done
done
