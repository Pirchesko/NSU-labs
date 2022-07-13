#include <stdio.h>
#include <stdlib.h>
#include <cblas.h>
#include <time.h>

#define N 1200
#define M 10

void random_matrix(float* A)
{
	for (int i = 0; i < N * N; i++)
	{
		A[i] = (float)(rand() % 18 - 9);
	}
}

void print_matrix(float* A)
{
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			printf("%g ", A[i * N + j]);
		}
		printf("\n");
	}
	printf("\n");
}

float* create_I()
{
	float* I = (float*)malloc(N * N * sizeof(float));
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if (i != j)
			{
				I[i * N + j] = 0;
			}
			else
			{
				I[i * N + j] = 1;
			}
		}
	}
	return I;
}

float* transposition(float* A)
{
	float* A_t = (float*)malloc(N * N * sizeof(float));
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			A_t[i * N + j] = A[j * N + i];
		}
	}
	return A_t;
}

float* create_B(float* A)
{
	float i_sum;
	float j_sum;
	float i_sum_max = 0;
	float j_sum_max = 0;
	for (int i = 0; i < N; i++)
	{
		i_sum = 0;
		j_sum = 0;
		for (int j = 0; j < N; j++)
		{
			i_sum += abs(A[j * N + i]);
			j_sum += abs(A[i * N + j]);
		}
		if (i_sum > i_sum_max)
		{
			i_sum_max = i_sum;
		}
		if (j_sum > j_sum_max)
		{
			j_sum_max = j_sum;
		}
	}
	float max_mul = i_sum_max * j_sum_max;
	float* A_t = transposition(A);
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			A_t[i * N + j] /= max_mul;
		}
	}
	return A_t;
}

void add(float* Res, float* X, float* Y)
{
	for (int i = 0; i < N * N; i++)
	{
		Res[i] = X[i] + Y[i];
	}
}

void sub(float* Res, float* X, float* Y)
{
	for (int i = 0; i < N * N; i++)
	{
		Res[i] = X[i] - Y[i];
	}
}

void mul(float* Res, float* X, float* Y)
{
	float* Result = (float*)malloc(N * N * sizeof(float));
	cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, N, N, N, 1.0, X, N, Y, N, 0.0, Result, N);
	for (int i = 0; i < N * N; i++)
	{
		Res[i] = Result[i];
	}
	free(Result);
}

float* inverse(float* A)
{
	float* I = create_I(); //I - единичная матрица
	float* B = create_B(A); //B = A_t / (max_mul)
	float* BA = (float*)malloc(N * N * sizeof(float));
	mul(BA, B, A); //B * A
	float* R = (float*)malloc(N * N * sizeof(float));
	sub(R, I, BA); //R = I - BA
	float* R_i = (float*)malloc(N * N * sizeof(float));
	sub(R_i, I, BA); //R_i = R = I - BA (для R^2, R^3, ...)
	float* A_ = (float*)malloc(N * N * sizeof(float));
	add(A_, I, R); //Обратная матрица A^-1
	for (int i = 1; i < M; i++)
	{
		mul(R_i, R_i, R);
		add(A_, A_, R_i);
	}
	mul(A_, A_, B);
	free(I);
	free(B);
	free(BA);
	free(R_i);
	free(R);
	free(A);
	return A_;
}

int main()
{
	struct timespec start, finish;
	clock_gettime(CLOCK_MONOTONIC_RAW, &start);
	float* A = (float*)malloc(N * N * sizeof(float));
	random_matrix(A);
	//print_matrix(A);
	A = inverse(A);
	//print_matrix(A); 
	free(A);
	clock_gettime(CLOCK_MONOTONIC_RAW, &finish);
	printf("Time: %lf\n\n", (finish.tv_sec - start.tv_sec + 0.000000001 * (finish.tv_nsec - start.tv_nsec)));
	return 0;
}