#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "mpi.h"
//#include <string.h>

//multyply matrixes: C=A*B
void multiply(int* C, int* A, int* B, int n1, int n2, int n3)
{
	for (int i = 0; i < n1; i++)
	{
		for (int j = 0; j < n2; j++)
		{
			for (int k = 0; k < n3; k++)
			{
				C[i * n3 + k] += A[i * n2 + j] * B[j * n3 + k];
			}
		}
	}
}

void random_matrix(int* M, int size)
{
	for (int i = 0; i < size; i++)
	{
		M[i] = rand() % 18 - 9;
	}
}

void print_matrix(int* M, int h, int l)
{
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < l; j++)
		{
			printf("%3d ", M[i * l + j]);
		}
		printf("\n");
	}
}

void go_mpi(int argc, char* argv[], int n1, int n2, int n3, int p1, int p2)
{
	MPI_Init(&argc, &argv);
	int ranks;
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &ranks);
	printf("Threads: %d\tNumber: %d\n", ranks, rank);
	int* A = NULL;
	int* B = NULL;
	int* C = NULL;
	int* A_ = (int*)malloc((n1 / p1) * n2 * sizeof(int));
	int* B_ = (int*)malloc(n2 * (n3 / p2) * sizeof(int));
	int* C_ = (int*)malloc((n1 / p1) * (n3 / p2) * sizeof(int));

	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Datatype B_vector;
	if (rank == 0)
	{
		printf("\nType vector\n");
		printf("Size: %d\n", n2);
		printf("Block: %d\n", n3 / p2);
		printf("Step: %d\n\n", n3);
	}
	MPI_Type_vector(n2, n3 / p2, n3, MPI_INT, &B_vector);
	MPI_Type_commit(&B_vector);

	MPI_Barrier(MPI_COMM_WORLD);
	if (rank == 0)
	{
		//Создаём и выводим матрицы
		A = (int*)malloc(n1 * n2 * sizeof(int));
		B = (int*)malloc(n2 * n3 * sizeof(int));
		C = (int*)malloc(n1 * n3 * sizeof(int));
		random_matrix(A, n1 * n2);
		random_matrix(B, n2 * n3);
		memset(C, 0, n1 * n3 * sizeof(int));
		printf("\nA: \n");
		print_matrix(A, n1, n2);
		printf("\nB: \n");
		print_matrix(B, n2, n3);

		//Инициализируем мелкие массивы (A_ и B_) для 0-ого процесса
		for (int i = 0; i < (n1 / p1) * n2; i++)
		{
			A_[i] = A[i];
		}
		for (int i = 0; i < n2; i++)
		{
			for (int j = 0; j < (n3 / p2); j++)
			{
				B_[i + j] = B[i * n3 + j];
			}
		}

		//Рассылка процессам матриц A_ и B_
		for (int i = 0; i < p2; i++)
		{
			for (int j = 0; j < p1; j++)
			{
				if (j + i != 0)
				{
					//printf("i = %d; j = %d\n", i, j);
					//printf("A[%d] = %d\n", (j * ((n1 / p1) * n2)), A[j * ((n1 / p1) * n2)]);
					//printf("Sjze: %d\n", (n1 / p1) * n2);
					//printf("Send ID: %d\n", (i * p1) + j);
					//printf("Tag: %d\n\n", (i * p1) + j);
					MPI_Send(&A[j * ((n1 / p1) * n2)], (n1 / p1) * n2, MPI_INT, (i * p1) + j, (i * p1) + j, MPI_COMM_WORLD);
				}
				else
				{
					//printf("i = %d; i= %d\n", i, j);
					//printf("skipped\n\n");
				}
			}
		}
		for (int i = 0; i < p2; i++)
		{
			for (int j = 0; j < p1; j++)
			{
				if (i + j != 0)
				{
					//printf("i = %d; j = %d\n", i, j);
					//printf("B[%d] = %d\n", i * (n3 / p2), B[i * (n3 / p2)]);
					//printf("Size: %d\n", (n2 * (n3 / p2)));
					//printf("Send ID: %d\n", (i * p1 + j));
					//printf("Tag: %d\n\n", (i * p1 + j) + 16);
					MPI_Send(&B[i * (n3 / p2)], 1, B_vector, i * p1 + j, (i * p1 + j) + 16, MPI_COMM_WORLD);
				}
				else
				{
					//printf("i = %d; j = %d\n", i, j);
					//printf("skipped\n\n");
				}
			}
		}
	}

	//Принимаем кусочки матриц A_ и выводим их для проверки
	MPI_Barrier(MPI_COMM_WORLD);
	if (rank != 0)
	{
		MPI_Recv(A_, (n1 / p1) * n2, MPI_INT, 0, rank, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	}
	MPI_Barrier(MPI_COMM_WORLD);
	for (int i = 0; i < p1 * p2; i++)
	{
		MPI_Barrier(MPI_COMM_WORLD);
		if (rank == i)
		{
			printf("Number: %d\n", rank);
			for (int h = 0; h < n1 / p1; h++)
			{
				for (int l = 0; l < n2; l++)
				{
					printf("%3d ", A_[h * n2 + l]);
				}
				printf("\n");
			}
		}
	}

	//Принимаем кусочки матриц B_ и выводим их для проверки
	MPI_Barrier(MPI_COMM_WORLD);
	if (rank != 0)
	{
		MPI_Recv(B_, n2 * (n3 / p2), MPI_INT, 0, rank + 16, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	}
	MPI_Barrier(MPI_COMM_WORLD);
	for (int i = 0; i < p1 * p2; i++)
	{
		MPI_Barrier(MPI_COMM_WORLD);
		if (rank == i)
		{
			printf("Number: %d:\n", rank);
			for (int h = 0; h < n3 / p2; h++)
			{
				for (int l = 0; l < n2; l++)
				{
					printf("%3d ", B_[l * n2 + h]);
				}
				printf("\n");
			}
			/*for (int i = 0; i < n2 * (n3 / p2); i++)
			{
				printf("%3d ", B_[i]);
			}*/
		}
	}

	//Зачищаем следы
	MPI_Type_free(&B_vector);
	MPI_Finalize();
	free(A_);
	free(B_);
	free(C_);
	free(A);
	free(B);
	free(C);
}

int main(int argc, char* argv[])
{
	int n1 = 8;
	int n2 = 5;
	int n3 = 4;
	int p1 = 4;
	int p2 = 2;

	if ((n1 % p1 != 0) || (n3 % p2 != 0))
	{
		printf("Error in p1 or p2!\n");
		return 0;
	}
	if (p1 * p2 > 16)
	{
		printf("Big number of p1 or p2\n");
		return 0;
	}
	else
	{
		go_mpi(argc, argv, n1, n2, n3, p1, p2);
	}

	return 0;
}

//https://stackoverflow.com/questions/13521129/receiving-with-mpi-type-vector-in-c