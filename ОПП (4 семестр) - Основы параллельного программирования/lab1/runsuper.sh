#!/bin/bash

#PBS -l walltime=00:10:00
#PBS -l select=1:ncpus=8:mpiprocs=8:mem=2000m,place=free
#PBS -m n
cd $PBS_O_WORKDIR
MPI_NP=$(wc -l $PBS_NODEFILE | awk '{ print $1 }')
echo "Number of MPI process: $MPI_NP"
echo 'File $PBS_NODEFILE:'
cat $PBS_NODEFILE
echo
./lab1.exe
mpirun -machinefile $PBS_NODEFILE -np 1 ./lab1_1.exe
mpirun -machinefile $PBS_NODEFILE -np 2 ./lab1_1.exe
mpirun -machinefile $PBS_NODEFILE -np 4 ./lab1_1.exe
mpirun -machinefile $PBS_NODEFILE -np 8 ./lab1_1.exe
mpirun -machinefile $PBS_NODEFILE -np 1 ./lab1_2.exe
mpirun -machinefile $PBS_NODEFILE -np 2 ./lab1_2.exe
mpirun -machinefile $PBS_NODEFILE -np 4 ./lab1_2.exe
mpirun -machinefile $PBS_NODEFILE -np 8 ./lab1_2.exe