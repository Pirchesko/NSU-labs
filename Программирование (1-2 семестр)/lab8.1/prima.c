#pragma warning(disable : 4996)
#include <stdio.h>
#include <malloc.h>
#include <limits.h>

//Проверяем и вводим данные
int input_edges(unsigned int* graph, int N, int M)
{
	int from;
	int to;
	int size;
	if ((N < 0) || (N > 5000))
	{
		return 1;
	}
	if ((M < 0) || (M > N * (N + 1) / 2))
	{
		return 2;
	}
	if (N == 0)
	{
		return 6;
	}
	for (int i = 0; i < (N * N); i++)
	{
		graph[i] = UINT_MAX;
	}
	for (int i = 0; i < M; i++)
	{
		if (scanf("%d%d%d", &from, &to, &size) != 3)
		{
			return 5;
		}
		if ((size < 0) || (size > INT_MAX))
		{
			return 4;
		}
		if ((from < 1) || (to < 1) || (from > N) || (to > N))
		{
			return 3;
		}
		graph[(from - 1) * N + (to - 1)] = size;
		graph[(to - 1) * N + (from - 1)] = size;
	}
	return 0;
}

//Алгоритм Прима
int algoritm_prima(unsigned* graph, int* used, unsigned int* min_e, int* sel_e, int* answer, int N)
{
	for (int i = 0; i < N; i++)
	{
		used[i] = 0;
		min_e[i] = UINT_MAX;
		sel_e[i] = -1;
	}
	min_e[0] = 0;
	int count = 0;
	for (int i = 0; i < N; i++)
	{
		int v = -1;
		for (int j = 0; j < N; j++)
		{
			if ((used[j] == 0) && ((v == -1) || (min_e[j] < min_e[v])))
			{
				v = j;
			}
		}
		if (min_e[v] == UINT_MAX)
		{
			return 6;
		}
		used[v] = 1;
		if (sel_e[v] != -1)
		{
			answer[count] = v;
			answer[count + 1] = sel_e[v];
			count = count + 2;
		}
		for (int j = 0; j < N; j++)
		{
			if (graph[v * N + j] < min_e[j])
			{
				min_e[j] = graph[v * N + j];
				sel_e[j] = v;
			}
		}
	}
	return 0;
}

//Выводим ответ
void print(int* answer, int N)
{
	for (int i = 0; i < (N - 1) * 2; i = i + 2)
	{
		printf("%d %d\n", answer[i] + 1, answer[i + 1] + 1);
	}
}

int main()
{
	int N;											// 0 - всё ок
	int M;											// 1 - bad number of vertices
	int error_code; //код возврата данных:			// 2 - bad number of edges
													// 3 - bad vertex
	//Ввод N и M									// 4 - bad length
	if (scanf("%d %d", &N, &M) != 2)				// 5 - bad number of lines
	{												// 6 - no spanning tree
		printf("bad number of lines");
		return 0;
	}

	unsigned int* graph = (unsigned int*)malloc((N * N) * sizeof(unsigned int));
	int* used = (int*)malloc(N * sizeof(int));
	unsigned int* min_e = (unsigned int*)malloc(N * sizeof(unsigned int));
	int* sel_e = (int*)malloc(N * sizeof(int));
	int* answer = (int*)malloc((N * 2) * sizeof(int));

	//Заполняем и проверяем данные
	error_code = input_edges(graph, N, M);
	if (error_code == 0)
	{
		error_code = algoritm_prima(graph, used, min_e, sel_e, answer, N);
	}

	switch (error_code)
	{
	case 0: print(answer, N); break;
	case 1: printf("bad number of vertices"); break;
	case 2: printf("bad number of edges"); break;
	case 3: printf("bad vertex"); break;
	case 4: printf("bad length"); break;
	case 5: printf("bad number of lines"); break;
	case 6: printf("no spanning tree"); break;
	}

	free(graph);
	free(used);
	free(min_e);
	free(sel_e);
	free(answer);

	return 0;
}
