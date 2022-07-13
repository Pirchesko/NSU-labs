#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define N 32
#define offset 4194304 //(16*1024*1024/4)

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

void do_zero(int* chache, int size)
{
	for (int i = 0; i < size; i++)
	{
		chache[i] = 0;
	}
}

void print_fragments()
{
	for (int i = 1; i <= 32; i++)
	{
		printf("%2d; ", i);
	}
	printf("\n");
}

void create_massive(int* chache, int block_size, int blocks)
{
	for (int i = 0; i < block_size; i++)
	{
		int k = i;
		for (int j = 0; j < blocks - 1; j++)
		{
			chache[k] = k + offset;
			k += offset;
		}
		if (k + 1 != block_size + offset * (blocks - 1))
		{
			chache[k] = i + 1;
		}
		else
		{
			chache[k] = 0;
		}
	}
}

int go(int* M, int n)
{
	int k = 0;
	for (int i = 0; i < n; i++)
	{
		k = M[k];
	}
	return k;
}

unsigned long long do_go(int* M, int n, int* check)
{
	unsigned long long time = rdtsc();
	for (int i = 0; i < 50; i++)
	{
		*check += go(M, n);
	}
	time = (rdtsc() - time) / (unsigned long long)(n*50);
	return time;
}

int main()
{
	int result = 0;
	int size_L3 = 3072 * 1024 / 4;
	print_fragments();
	for (int k = 0; k < 10; k++)
	{
		for (int i = 1; i <= 32; i++)
		{
			int chache_size = offset * i;
			int* chache = (int*)malloc(chache_size * sizeof(int));
			do_zero(chache, chache_size);
			create_massive(chache, (size_L3 / i), i);
			printf("%2llu; ", do_go(chache, (size_L3 - (size_L3 % i)), &result));
			free(chache);
		}
		printf("%d \n", result);
	}
	return 0;
}
//https://ru.wikipedia.org/wiki/%D0%9A%D1%8D%D1%88#%D0%90%D1%81%D1%81%D0%BE%D1%86%D0%B8%D0%B0%D1%82%D0%B8%D0%B2%D0%BD%D0%BE%D1%81%D1%82%D1%8C_%D0%BA%D1%8D%D1%88%D0%B0
//https://habr.com/ru/post/93263/
//https://compress.ru/article.aspx?id=23541

//#include <stdio.h>
//#include <stdlib.h>
//#include <limits.h>
//
//#define N 32
//#define offset 4194304 //(16*1024*1024/4)
////#define offset 256 //(16*1024*1024/4)
//
//#ifdef __i386
//__inline__ unsigned long long rdtsc()
//{
//	unsigned long long x;
//	__asm__ volatile ("rdtsc" : "=A" (x));
//	return x;
//}
//#elif __amd64
//__inline__ unsigned long long rdtsc()
//{
//	unsigned long long a, d;
//	__asm__ volatile ("rdtsc" : "=a" (a), "=d" (d));
//	return (d << 32) | a;
//}
//#endif
//
//void print(int* M, int chache_size)
//{
//	for (int i = 0; i < chache_size; i++)
//	{
//		printf("M[%d] = %d\n", i, M[i]);
//	}
//	printf("\n");
//}
//
//void get_next_n(int* n, int* q)
//{
//	if (*n % *q == 0)
//	{
//		*n = *n + *n / 2;
//	}
//	else
//	{
//		*n = *n / 3 * 4;
//		*q = *n;
//	}
//}
//
//void print_fragments()
//{
//	for (int i = 1; i <= 32; i++)
//	{
//		printf("%2d; ", i);
//	}
//	printf("\n");
//}
//
//void create_massive(int* chache, int block_size, int blocks)
//{
//	for (int i = 0; i < block_size; i++)
//	{
//		int k = i;
//		for (int j = 0; j < blocks - 1; j++)
//		{
//			chache[k] = k + offset;
//			k += offset;
//		}
//		if (k + 1 != block_size + offset * (blocks - 1))
//		{
//			chache[k] = i + 1;
//		}
//		else
//		{
//			chache[k] = 0;
//		}
//		/*while (k + offset < chache_size)
//		{
//			M[k] = k + to_next;
//			k += to_next;
//		}
//		if (k + 1 != size - heap)
//		{
//			M[k] = i + 1;
//		}
//		else
//		{
//			M[k] = 0;
//		}*/
//	}
//}
//
//int go_test(int* chache, int elements, int blocks)
//{
//	int k = 0;
//	int* S = (int*)malloc(elements * sizeof(int));
//	for (int i = 0; i < elements; i++)
//	{
//		S[i] = 0;
//	}
//	for (int i = 0; i < elements; i++)
//	{
//		k = chache[k];
//		S[i] = k;
//	}
//	int c = 0;
//	for (int i = 0; i < elements; i++)
//	{
//		c++;
//		if (c == blocks)
//		{
//			printf("%10d\n ", S[i]);
//			c = 0;
//		}
//		else
//		{
//			printf("%10d ", S[i]);
//		}
//	}
//	free(S);
//	return k;
//}
//
//int go(int* M, int n)
//{
//	int k = 0;
//	for (int i = 0; i < n; i++)
//	{
//		k = M[k];
//	}
//	return k;
//}
//
//unsigned long long do_go(int* M, int n, int* check)
//{
//	unsigned long long min = ULLONG_MAX;
//	for (int i = 0; i < 10; i++)
//	{
//		unsigned long long time = rdtsc();
//		*check += go(M, n);
//		time = (rdtsc() - time) / (unsigned long long)n;
//		if (min > time)
//		{
//			min = time;
//		}
//	}
//	return min;
//}
//
//int main()
//{
//	//int chache_size = 16 * 1024 * 1024 / 4; //типо 16 мб в int
//
//	int result = 0;
//	/*printf("fragmets; ");
//	for (int j = 1; j <= N; j++)
//	{
//		if (j != N)
//		{
//			printf("%d; ", j);
//		}
//		else
//		{
//			printf("%d\n", j);
//		}
//	}*/
//	int chache_size = 16 * 33 * 1024 * 1024 / 4;
//	int* chache = (int*)malloc(chache_size * sizeof(int));
//	int max_associativity = 48;
//	int n;
//	int q;
//
//	//Go Go GO
//	printf("For L1:\n");
//	int size_L1 = 32 * 1024 / 4; //кол-во элементов в int
//	n = 2;
//	q = n;
//	for (int n = 2; n <= max_associativity; get_next_n(&n, &q))
//	{
//		if (size_L1 % n == 0)
//		{
//			printf("%d associativity: \n", n);
//			printf("fragments; ");
//			print_fragments();
//			printf("tacts;     ");
//			for (int j = 1; j <= 32; j++)
//			{
//				int block_size = size_L1 / n;
//				create_massive(chache, block_size, j);
//				//printf("tests: %d\n", go_test(chache, (block_size * j), j));
//				printf("%2llu; ", do_go(chache, (block_size * j), &result));
//			}
//			printf("\n");
//		}
//	}
//	printf("For L2:\n");
//	int size_L2 = 256 * 1024 / 4; //кол-во элементов в int
//	n = 2;
//	q = n;
//	for (int n = 2; n <= max_associativity; get_next_n(&n, &q))
//	{
//		if (size_L2 % n == 0)
//		{
//			printf("%d associativity: \n", n);
//			printf("fragments; ");
//			print_fragments();
//			printf("tacts;     ");
//			for (int j = 1; j <= 32; j++)
//			{
//				int block_size = size_L2 / n;
//				create_massive(chache, block_size, j);
//				//printf("tests: %d\n", go_test(chache, (block_size * j), j));
//				printf("%2llu; ", do_go(chache, (block_size * j), &result));
//			}
//			printf("\n");
//		}
//	}
//	printf("For L3:\n");
//	int size_L3 = 3072 * 1024 / 4; //кол-во элементов в int
//	n = 2;
//	q = n;
//	for (int n = 2; n <= max_associativity; get_next_n(&n, &q))
//	{
//		if (size_L3 % n == 0)
//		{
//			printf("%d associativity: \n", n);
//			printf("fragments; ");
//			print_fragments();
//			printf("tacts;     ");
//			for (int j = 1; j <= 32; j++)
//			{
//				int block_size = size_L3 / n;
//				create_massive(chache, block_size, j);
//				//printf("tests: %d\n", go_test(chache, (block_size * j), j));
//				printf("%2llu; ", do_go(chache, (block_size * j), &result));
//			}
//			printf("\n");
//		}
//	}
//	printf("Result: %d\n", result);
//	free(chache);
//	return 0;
//}