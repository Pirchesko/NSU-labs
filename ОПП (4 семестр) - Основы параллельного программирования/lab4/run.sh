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
echo "Without MPI"
icc lab4.c -std=gnu99 -o lab4.exe
./lab4.exe 6 6 6 100000000
echo " "
echo " MPI"
mpiicc lab4_mpi.c -std=gnu99 -o lab4_mpi.exe
mpirun -np 2 ./lab4_mpi.exe 6 6 6 100000000
echo " "
echo " "
echo " "
echo "Without MPI"
icc lab4.c -std=gnu99 -o lab4.exe
./lab4.exe 8 8 8 100000000
echo " "
echo " MPI"
mpiicc lab4_mpi.c -std=gnu99 -o lab4_mpi.exe
mpirun -np 2 ./lab4_mpi.exe 8 8 8 100000000
echo " "
echo " "
echo " "
echo "Without MPI"
icc lab4.c -std=gnu99 -o lab4.exe
./lab4.exe 10 10 10 100000000
echo " "
echo " MPI"
mpiicc lab4_mpi.c -std=gnu99 -o lab4_mpi.exe
mpirun -np 2 ./lab4_mpi.exe 10 10 10 100000000
echo " "
echo " "
echo " "
echo "Without MPI"
icc lab4.c -std=gnu99 -o lab4.exe
./lab4.exe 12 12 12 100000000
echo " "
echo " MPI"
mpiicc lab4_mpi.c -std=gnu99 -o lab4_mpi.exe
mpirun -np 2 ./lab4_mpi.exe 12 12 12 100000000
echo " "
echo " "
echo " "
echo "Without MPI"
icc lab4.c -std=gnu99 -o lab4.exe
./lab4.exe 14 14 14 100000000
echo " "
echo " MPI"
mpiicc lab4_mpi.c -std=gnu99 -o lab4_mpi.exe
mpirun -np 2 ./lab4_mpi.exe 14 14 14 100000000
echo " "
echo " "
echo " "
echo "Without MPI"
icc lab4.c -std=gnu99 -o lab4.exe
./lab4.exe 16 16 16 100000000
echo " "
echo " MPI"
mpiicc lab4_mpi.c -std=gnu99 -o lab4_mpi.exe
mpirun -np 2 ./lab4_mpi.exe 16 16 16 100000000
echo " "
echo " "
echo " "

