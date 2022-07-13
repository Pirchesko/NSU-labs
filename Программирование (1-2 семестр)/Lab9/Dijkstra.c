#pragma warning(disable : 4996)
#include <stdio.h>
#include <malloc.h>
#include <limits.h>

//Ввод, проверка и инициализация массивов
int input(int* graph, int* parent, int* used, long long* dlina, int* count_way, int N, int S, int F, int M)
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
	if ((S < 1) || (S > N) || (F < 1) || (F > N))
	{
		return 3;
	}
	for (int i = 0; i < (N * N); i++)
	{
		graph[i] = -1;
	}
	for (int i = 0; i < N; i++)
	{
		parent[i] = -1;
		dlina[i] = LLONG_MAX / 2;
		count_way[i] = 0;
		used[i] = 0;
	}
	dlina[S - 1] = 0;
	count_way[S - 1] = 1;
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

//Ищем следующую вершинку с наименьшим  расстоянием
int min_dlina(unsigned int* dlina, int* used, int N)
{
	long long min = LLONG_MAX / 2;
	int index = 0;
	for (int i = 0; i < N; i++)
	{
		if ((used[i] == 0) && (min > dlina[i]))
		{
			min = dlina[i];
			index = i;
		}
	}
	used[index] = 1;
	return index;
}

//Алгоритм Дейкстры
void algiritm_Dijkstra(int* graph, int* parent, int* used, long long* dlina, int* count_way, int N)
{
	for (int i = 0; i < N; i++)
	{
		int v = min_dlina(dlina, used, N);
		for (int j = 0; j < N; j++)
		{
			if (graph[v * N + j] != -1)
			{
				count_way[j] += count_way[v];
				if (dlina[v] + (long long)graph[v * N + j] < dlina[j])
				{
					dlina[j] = dlina[v] + graph[v * N + j];
					parent[j] = v;
				}
			}
		}
	}
}

//Выводим расстояния до каждой вершинки
void print_dlina(long long* dlina, int N)
{
	for (int i = 0; i < N; i++)
	{
		if (dlina[i] >= LLONG_MAX / 2)
		{
			printf("oo ");
		}
		else if (dlina[i] > INT_MAX)
		{
			printf("INT_MAX+ ");
		}
		else
		{
			printf("%u ", dlina[i]);
		}
	}
	printf("\n");
}

//Выводим маршрут
void print_way(long long* dlina, int* count_way, int* parent, int S, int F)
{
	if (dlina[F - 1] >= LLONG_MAX / 2)
	{
		printf("no path ");
	}
	else if ((dlina[F - 1] > INT_MAX) && (count_way[F-1] > 1))
	{
		printf("overflow ");
	}
	else
	{
		while (S != F)
		{
			printf("%d ", F);
			F = parent[F - 1] + 1;
		}
		printf("%d ", S);
	}
}

int main()
{
	int N;										// 0 - всё ок
	int S;										// 1 - bad number of vertices
	int F;										// 2 - bad number of edges
	int M;										// 3 - bad vertex
	int error_code; //код возрата ошибки		// 4 - bad length
												// 5 - bad number of lines
	if (scanf("%d%d%d%d", &N, &S, &F, &M) != 4)
	{
		printf("bad number of lines");
		return 0;
	}
	
	int* graph = (int*)malloc(N * N * sizeof(int)); //граф
	int* parent = (int*)malloc(N * sizeof(int)); //предки
	int* used = (int*)malloc(N * sizeof(int)); //проверка на посещяемость
	long long* dlina = (long long*)malloc(N * sizeof(long long)); //расстояние до вершин
	int* count_way = (int*)malloc(N * sizeof(int)); //количество маршрутов

	error_code = input(graph, parent, used, dlina, count_way, N, S, F, M);
	if (error_code == 0)
	{
		algiritm_Dijkstra(graph, parent, used, dlina, count_way, N);
		print_dlina(dlina, N);
		print_way(dlina, count_way, parent, S, F);
	}

	switch (error_code)
	{
	case 1: printf("bad number of vertices"); break;
	case 2: printf("bad number of edges"); break;
	case 3: printf("bad vertex"); break;
	case 4: printf("bad length"); break;
	case 5: printf("bad number of lines"); break;
	case 6: printf("no spanning tree"); break;
	}

	free(graph);
	free(parent);
	free(used);
	free(dlina);
	free(count_way);

	return 0;
}
