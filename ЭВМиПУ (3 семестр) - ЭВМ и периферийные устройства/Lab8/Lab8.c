#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 33

#ifdef __i386
__inline__ unsigned long long rdtsc()
{
	unsigned long long x;
	__asm__ volatile ("rdtsc" : "=A" (x));
	return x;
}
#elif __amd64
__inline__ unsigned long long rdtsc()
{
	unsigned long long a, d;
	__asm__ volatile ("rdtsc" : "=a" (a), "=d" (d));
	return (d << 32) | a;
}
#endif

void print(int* M, int n)
{
	for (int i = 0; i < n; i++)
	{
		printf("M[%d] = %d\n", i, M[i]);
	}
	printf("\n");
}

int* do_next(int* M, int n)
{
	for (int i = 0; i < n - 1; ++i)
	{
		M[i] = i + 1;
	}
	M[n - 1] = 0;
	return M;
}

int* do_previous(int* M, int n)
{
	for (int i = n - 1; i > 0; --i)
	{
		M[i] = i - 1;
	}
	M[0] = n - 1;
	return M;
}

int* do_random(int* M, int n)
{
	for (int i = 0; i < n; i++)
	{
		M[i] = i;
	}
	srand((int)time(0));
	int* M_ = (int*)malloc(n * sizeof(int));
	int r = rand() % n;
	int prev = r;
	int start = r;
	for (int i = 0; i < n - 1; i++)
	{
		M[r] = -1;
		r = rand() % n;
		while ((M[r] == -1))
		{
			if (r != n - 1)
			{
				r++;
			}
			else
			{
				r = 0;
			}
		}
		M_[prev] = r;
		prev = r;
	}
	M_[prev] = start;
	free(M);
	return M_;
}

int go(int* M, int n, int k)
{
	int a = 0;
	for (int i = 0; i < (n * k); ++i)
	{
		a = M[a];
	}
	return a;
}

void get_next_n(int* n, int* q)
{
	if (*n % *q == 0)
	{
		*n = *n + *n / 2;
	}
	else
	{
		*n = *n / 3 * 4;
		*q = *n;
	}
}

int main()
{
	int** results = (int**)malloc(3 * sizeof(int*));
	for (int i = 0; i < 3; i++)
	{
		results[i] = (int*)malloc(N * sizeof(int));
	}
	int r;
	int k = 10;
	int n = 256;
	int q = n;
	for (int i = 0; i < N; i++)
	{
		int c = n * 4 / 1024;
		if (c < 1024)
		{
			if (i != N - 1)
			{
				printf("%gKB; ", ((double)(n * 4) / (double)(1024)));
			}
			else
			{
				printf("%gKB\n", ((double)(n * 4) / (double)(1024)));
			}
		}
		else
		{
			if (i != N - 1)
			{
				printf("%gMB; ", ((double)(n * 4) / (double)(1024 * 1024)));
			}
			else
			{
				printf("%gMB\n", ((double)(n * 4) / (double)(1024 * 1024)));
			}
		}
		get_next_n(&n, &q);
	}
	n = 256;
	q = n;
	for (int i = 0; i < N; i++)
	{
		int* M = (int*)malloc(n * sizeof(int));
		M = do_next(M, n);
		r = go(M, n, 1);
		unsigned long long time = rdtsc();
		results[0][i] = go(M, n, k);
		time = (rdtsc() - time) / (unsigned long long)(n * k);
		if (i != N - 1)
		{
			printf("%llu; ", time);
		}
		else
		{
			printf("%llu\n", time);
		}
		free(M);
		get_next_n(&n, &q);
	}
	n = 256;
	q = n;
	for (int i = 0; i < N; i++)
	{
		int* M = (int*)malloc(n * sizeof(int));
		M = do_previous(M, n);
		r = go(M, n, 1);
		unsigned long long time = rdtsc();
		results[1][i] = go(M, n, k);
		time = (rdtsc() - time) / (unsigned long long)(n * k);
		if (i != N - 1)
		{
			printf("%llu; ", time);
		}
		else
		{
			printf("%llu\n", time);
		}
		free(M);
		get_next_n(&n, &q);
	}
	n = 256;
	q = n;
	for (int i = 0; i < N; i++)
	{
		int* M = (int*)malloc(n * sizeof(int));
		M = do_random(M, n);
		r = go(M, n, 1);
		unsigned long long time = rdtsc();
		results[2][i] = go(M, n, k);
		time = (rdtsc() - time) / (unsigned long long)(n * k);
		if (i != N - 1)
		{
			printf("%llu; ", time);
		}
		else
		{
			printf("%llu\n", time);
		}
		free(M);
		get_next_n(&n, &q);
	}
	r = 0;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < N; j++)
		{
			r += results[i][j];
		}
	}
	printf("Results (need for good work optomozation -O1): %d\n", r);
	for (int i = 0; i < 3; i++)
	{
		free(results[i]);
	}
	free(results);
	return 0;
}
//https://coderoad.ru/9887839/%D0%9A%D0%B0%D0%BA-%D1%81%D1%87%D0%B8%D1%82%D0%B0%D1%82%D1%8C-%D1%82%D0%B0%D0%BA%D1%82%D1%8B-%D1%81-RDTSC-%D0%B2-GCC-x86