#!/bin/bash

#PBS -l walltime=00:01:00
#PBS -l select=1:ncpus=4:mpiprocs=4:mem=2000m,place=free
#PBS -m n
cd $PBS_O_WORKDIR
MPI_NP=$(wc -l $PBS_NODEFILE | awk '{ print $1 }')
echo "Number of MPI process: $MPI_NP"
echo 'File $PBS_NODEFILE:'
cat $PBS_NODEFILE
echo
mpirun -machinefile $PBS_NODEFILE -np $MPI_NP ./lab1_1.exe