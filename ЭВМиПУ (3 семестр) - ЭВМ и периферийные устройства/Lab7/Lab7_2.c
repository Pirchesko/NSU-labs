#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <xmmintrin.h>

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
	float* I = (float*)_mm_malloc(N * N * sizeof(float), 16);
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
	float* A_t = (float*)_mm_malloc(N * N * sizeof(float), 16);
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
	int n = N * N - N * N % 4;
	for (int i = 0; i < n; i += 4)
	{
		_mm_store_ps(&Res[i], _mm_add_ps(_mm_load_ps(&X[i]), _mm_load_ps(&Y[i])));
	}
	for (int i = n; i < N * N; ++i)
	{
		Res[i] = X[i] + Y[i];
	}
}

void sub(float* Res, float* X, float* Y)
{
	int n = N * N - N * N % 4;
	for (int i = 0; i < n; i += 4)
	{
		_mm_store_ps(&Res[i], _mm_sub_ps(_mm_load_ps(&X[i]), _mm_load_ps(&Y[i])));
	}
	for (int i = n; i < N * N; ++i)
	{
		Res[i] = X[i] - Y[i];
	}
}

float mul_M(int i, int j, float* X, float* Y)
{
	float res = 0;
	__m128 sum = _mm_setzero_ps();
	__m128 mul;
	int n = N - N % 4;
	for (int k = 0; k < n; k += 4)
	{
		mul = _mm_mul_ps(_mm_loadu_ps(&X[k]), _mm_loadu_ps(&Y[k]));
		sum = _mm_add_ps(sum, mul);
	}
	if (n > 0)
	{
		mul = _mm_movehl_ps(mul, sum);
		sum = _mm_add_ps(sum, mul);
		mul = _mm_shuffle_ps(sum, sum, 1);
		sum = _mm_add_ss(sum, mul);
		_mm_store_ss(&res, sum);
	}
	if (N % 4 != 0)
	{
		for (int i = n; i < N; i++)
		{
			res += X[i] * Y[i];
		}
	}
	return res;
}

void mul(float* Res, float* X, float* Y)
{
	float* Result = (float*)_mm_malloc(N * N * sizeof(float), 16);
	Y = transposition(Y);
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			Result[i * N + j] = mul_M(i, j, &X[i * N], &Y[j * N]);
		}
	}
	for (int i = 0; i < N * N; i++)
	{
		Res[i] = Result[i];
	}
	_mm_free(Result);
}

void mul_2(float* Res, float* X, float* Y)
{
	float* Result = (float*)malloc(N * N * sizeof(float));
	__m128 zero = _mm_setzero_ps();
	int n = N * N - N * N % 4;
	for (int i = 0; i < n; i += 4)
	{
		_mm_store_ps(&Result[i], zero);
	}
	for (int i = n; i < N * N; ++i)
	{
		Result[i] = 0;
	}
	int n_ = N - N % 4;
	for (int i = 0; i < n_; i++)
	{
		for (int k = 0; k < n_; k++)
		{
			//Result[i * N + j] = 0;
			for (int j = 0; j < n_; j+=4)
			{
				//Result[i * N + j] += X[i * N + k] * Y[k * N + j];
				_mm_store_ps(&Result[i * N + j], _mm_mul_ps(_mm_set1_ps(X[i * N + k]), _mm_loadu_ps(&Y[k * N + j])));
			}
		}
	}
	for (int i = n_; i < N; i++)
	{
		for (int k = n_; k < N; k++)
		{
			for (int j = n_; j < N; j++)
			{
				Result[i * N + j] += X[i * N + k] * Y[k * N + j];
			}
		}
	}
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
	float* BA = (float*)_mm_malloc(N * N * sizeof(float), 16);
	mul_2(BA, B, A); //B * A
	float* R = (float*)_mm_malloc(N * N * sizeof(float), 16);
	sub(R, I, BA); //R = I - BA
	float* R_i = (float*)_mm_malloc(N * N * sizeof(float), 16);
	sub(R_i, I, BA);
	float* A_ = (float*)_mm_malloc(N * N * sizeof(float), 16);
	add(A_, I, R); //Обратная матрица A^-1
	for (int i = 1; i < M; i++)
	{
		mul_2(R_i, R_i, R);
		add(A_, A_, R_i);
	}
	mul_2(A_, A_, B);
	_mm_free(I);
	_mm_free(B);
	_mm_free(BA);
	_mm_free(R_i);
	_mm_free(R);
	_mm_free(A);
	return A_;
}

int main()
{
	struct timespec start, finish;
	clock_gettime(CLOCK_MONOTONIC_RAW, &start);
	float* A = (float*)_mm_malloc(N * N * sizeof(float), 16);
	random_matrix(A);
	//print_matrix(A);
	A = inverse(A);
	//print_matrix(A);
	_mm_free(A);
	clock_gettime(CLOCK_MONOTONIC_RAW, &finish);
	printf("Time: %lf\n\n", (finish.tv_sec - start.tv_sec + 0.000000001 * (finish.tv_nsec - start.tv_nsec)));
	return 0;
}