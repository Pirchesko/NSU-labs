#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <time.h>
#include "mpi.h"

void random_matrix(int* M, int size)
{
	for (int i = 0; i < size; i++)
	{
		M[i] = rand() % 18 - 9;
	}
}

void print_matrix(int* M, int h, int l, int rank)
{
	if (rank == 0)
	{
		for (int i = 0; i < h; i++)
		{
			for (int j = 0; j < l; j++)
			{
				printf("%4d ", M[i * l + j]);
			}
			printf("\n");
		}
	}
}

//Просто проверка, правильно ли посчитала матрица
void check_result(MPI_Comm comm2d, int* C, int* C_, int rank, int n1, int n3, int p1, int p2)
{
	long long int check = 0;
	if (rank == 0)
	{
		for (int i = 0; i < n1 * n3; i++)
		{
			check += C[i];
		}
		printf("Check: %lld\n", check);
	}
	long long int check1 = 0;
	long long int check2 = 0;
	for (int i = 0; i < (n1 / p1) * (n3 / p2); i++)
	{
		check1 += C_[i];
	}
	MPI_Reduce(&check1, &check2, 1, MPI_LONG_LONG_INT, MPI_SUM, 0, comm2d);
	if (rank == 0)
	{
		printf("sum = %lld\n", check2);
	}
}

//Сбор всех подматриц в матрицу
void unite_matrix(MPI_Comm comm2d, int* C, int* C_, int rank, int size, int n1, int n2, int n3, int p1, int p2)
{
	MPI_Datatype C_vector;
	MPI_Type_vector(n1 / p1, n3 / p2, n3, MPI_INT, &C_vector);
	MPI_Type_commit(&C_vector);
	MPI_Datatype C_vector_;
	MPI_Type_create_resized(C_vector, 0, (n3 / p2) * sizeof(int), &C_vector_);
	MPI_Type_commit(&C_vector_);

	int* recvcounts = (int*)malloc(size * sizeof(int));
	int* displs = (int*)malloc(size * sizeof(int));
	for (int i = 0; i < p1; i++)
	{
		for (int j = 0; j < p2; j++)
		{
			displs[i * p2 + j] = j + i * p2 * (n1 / p1);
			recvcounts[i * p2 + j] = 1;
		}
	}
	//MPI_Barrier(comm2d);
	MPI_Gatherv(C_, (n1 / p1) * (n3 / p2), MPI_INT, C, recvcounts , displs , C_vector_, 0, comm2d);
	
	MPI_Type_free(&C_vector);
	MPI_Type_free(&C_vector_);
	free(recvcounts);
	free(displs);
}

//Умножения матриц
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

//Отправка кусков матриц в процессы
void send_chunks(MPI_Comm comm2d, int size, int rank, int* A, int* B, int* C, int* A_, int* B_, int* C_, int n1, int n2, int n3, int p1, int p2)
{
	int color_A = rank / p2;
	int color_B = rank % p2;
	int sizeA;
	int sizeB;
	int rankA;
	int rankB;
	MPI_Comm commA;
	MPI_Comm commB;
	MPI_Comm_split(comm2d, color_A, color_B, &commB);
	MPI_Comm_split(comm2d, color_B, color_A, &commA);
	MPI_Comm_rank(commA, &rankA);
	MPI_Comm_rank(commB, &rankB);
	MPI_Comm_size(commA, &sizeA);
	MPI_Comm_size(commB, &sizeB);

	MPI_Datatype B_vector;
	MPI_Type_vector(n2, n3 / p2, n3, MPI_INT, &B_vector);
	MPI_Type_commit(&B_vector);
	MPI_Datatype B_vector_;
	MPI_Type_create_resized(B_vector, 0, (n3 / p2) * sizeof(int), &B_vector_);
	MPI_Type_commit(&B_vector_);

	if (rank == rankA * sizeB)
	{
		MPI_Scatter(A, (n1 / p1) * n2, MPI_INT, A_, (n1 / p1) * n2, MPI_INT, 0, commA);
	}
	MPI_Bcast(A_, (n1 / p1) * n2, MPI_INT, 0, commB);
	if (rank == rankB)
	{
		MPI_Scatter(B, 1, B_vector_, B_, n2 * (n3 / p2), MPI_INT, 0, commB);
	}
	MPI_Bcast(B_, n2 * (n3 / p2), MPI_INT, 0, commA);

	MPI_Comm_free(&commA);
	MPI_Comm_free(&commB);
	MPI_Type_free(&B_vector);
	MPI_Type_free(&B_vector_);
}

//Выделение памяти для процессов
void memory_matrix(int** A, int** B, int** C, int** A_, int** B_, int** C_, int rank, int n1, int n2, int n3, int p1, int p2)
{
	*A = NULL;
	*B = NULL;
	*C = NULL;
	*A_ = (int*)malloc((n1 / p1) * n2 * sizeof(int));
	*B_ = (int*)malloc(n2 * (n3 / p2) * sizeof(int));
	*C_ = (int*)malloc((n1 / p1) * (n3 / p2) * sizeof(int));
	memset(*C_, 0, (n1 / p1) * (n3 / p2) * sizeof(int));
	if (rank == 0)
	{
		*A = (int*)malloc(n1 * n2 * sizeof(int));
		*B = (int*)malloc(n2 * n3 * sizeof(int));
		*C = (int*)malloc(n1 * n3 * sizeof(int));
		random_matrix(*A, n1 * n2);
		random_matrix(*B, n2 * n3);
	}
}

//Создание топологии
void topology(MPI_Comm* comm2d, int* rank, int* size, int p1, int p2)
{
	int dims[2] = { p1,p2 }; //решётка p1 на p2
	int periods[2] = { 0,0 }; //переодичность (непереодическая)
	int reorder = 1; //указываает, производить ли перенумерацию процессов
	MPI_Comm_size(MPI_COMM_WORLD, size); 
	MPI_Dims_create(*size, 2, dims); //определение размера решётки
	MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, reorder, comm2d); //создание решётки
	MPI_Comm_rank(*comm2d, rank); //забираем, вероятно, новые номера процессов
}

//Здесь начинаются все алгоритмы
void go(int n1, int n2, int n3, int p1, int p2)
{
	MPI_Comm comm2d; //коммуникатор решётки
	int rank, size;
	topology(&comm2d, &rank, &size, p1, p2); //создаём топологию
	int* A, *B, *C, *A_, *B_, *C_;
	memory_matrix(&A, &B, &C, &A_, &B_, &C_, rank, n1, n2, n3, p1, p2); //выделяем память и создаём матрицы
	send_chunks(comm2d, size, rank, A, B, C, A_, B_, C_, n1, n2, n3, p1, p2); //рассылка кусочков всем процессам
	multiply(C_, A_, B_, n1 / p1, n2, n3 / p2); //перемножение матриц
	unite_matrix(comm2d, C, C_, rank, size, n1, n2, n3, p1, p2); //сбор матриц в единое целлое
	//printf("\nC: \n");
	//print_matrix(C, n1, n3, rank);
	check_result(comm2d, C, C_, rank, n1, n3, p1, p2); //проверка результатов
	MPI_Comm_free(&comm2d);
	free(A);
	free(B);
	free(C);
	free(A_);
	free(B_);
	free(C_);
}

int main(int argc, char* argv[])
{
	int n1 = 5040;
	int n2 = 7560;
	int n3 = 10080;
	int p1 = 4;
	int p2 = 4;

	if ((argv[1] != 0) && (argv[2] != 0))
	{
		p1 = atoi(argv[1]);
		p2 = atoi(argv[2]);
	}
	if ((argv[3] != 0) && (argv[4] != 0) && (argv[5] != 0))
	{
		n1 = atoi(argv[3]);
		n2 = atoi(argv[4]);
		n3 = atoi(argv[5]);
	}
	if (p1 * p2 > 16)
	{
		printf("Big number of p1 or p2 (p1 = %d; p2 = %d)\n", p1, p2);
		return 0;
	}
	if ((p1 < 1) || (p2 < 1))
	{
		printf("Small number of p1 or p2 (p1 = %d; p2 = %d)\n", p1, p2);
		return 0;
	}
	if ((n1 % p1 != 0) || (n3 % p2 != 0))
	{
		printf("Error in p1 or p2!\n");
		return 0;
	}

	MPI_Init(&argc, &argv);
	int size;
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	printf("Threads: %d\tNumber: %d\n", size, rank);

	if (size != p1 * p2)
	{
		if (rank == 0)
		{
			printf("Error! Threads is %d, p1*p2=%d\n", size, p1 * p2);
		}
	}
	else
	{
		float time;
		float time_min;
		struct timespec start, finish;
		clock_gettime(CLOCK_MONOTONIC_RAW, &start);
		go(n1, n2, n3, p1, p2); //все возможные ошибки проверили - можно начинать!
		clock_gettime(CLOCK_MONOTONIC_RAW, &finish);
		time = finish.tv_sec - start.tv_sec + 0.000000001 * (finish.tv_nsec - start.tv_nsec);
		MPI_Reduce(&time, &time_min, 1, MPI_FLOAT, MPI_MIN, 0, MPI_COMM_WORLD);
		if (rank == 0)
		{
			printf("Time: %lf \n", time);
		}
	}

	MPI_Finalize();

	return 0;
}