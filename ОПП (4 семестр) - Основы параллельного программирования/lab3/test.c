#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <time.h>

//multyply matrixes: C=A*B
void multiply(int* C, int* A, int* B, int n1, int n2, int n3)
{
	for (int i = 0; i < n1; i++)
	{
		for (int j = 0; j < n2; j++)
		{
			for (int k = 0; k < n3; k++)
			{
				//printf("C[%d][%d](%d) += A[%d][%d](%d) * B[%d][%d](%d)     ", i, k, i * n3 + k, i, j, i * n2 + j, j, k, j * n3 + k);
				C[i * n3 + k] += A[i * n2 + j] * B[j * n3 + k];
				//printf("(%d)\n", C[i * n3 + k]);
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
			printf("%4d ", M[i * l + j]);
		}
		printf("\n");
	}
}

int main(int argc, char* argv[])
{
	int n1 = 5040;
	int n2 = 7560;
	int n3 = 10080;

	if ((argv[1] != 0) && (argv[2] != 0) && (argv[3] != 0))
	{
		n1 = atoi(argv[1]);
		n2 = atoi(argv[2]);
		n3 = atoi(argv[3]);
	}

	float time;
	struct timespec start, finish;
	clock_gettime(CLOCK_MONOTONIC_RAW, &start);

	int* A = (int*)malloc(n1 * n2 * sizeof(int));
	int* B = (int*)malloc(n2 * n3 * sizeof(int));
	int* C = (int*)malloc(n1 * n3 * sizeof(int));
	random_matrix(A, n1 * n2);
	random_matrix(B, n2 * n3);
	memset(C, 0, n1 * n3 * sizeof(int));

	/*printf("\nA: \n");
	print_matrix(A, n1, n2);
	printf("\nB: \n");
	print_matrix(B, n2, n3);
	
	printf("\nC: \n");
	print_matrix(C, n1, n3);*/

	multiply(C, A, B, n1, n2, n3);

	long long int check = 0;
	for (int i = 0; i < n1 * n3; i++)
	{
		check += C[i];
	}
	printf("Check: %lld\n", check);

	clock_gettime(CLOCK_MONOTONIC_RAW, &finish);
	time = finish.tv_sec - start.tv_sec + 0.000000001 * (finish.tv_nsec - start.tv_nsec);
	printf("Time: %lf \n", time);

	free(A);
	free(B);
	free(C);
	return 0;
}