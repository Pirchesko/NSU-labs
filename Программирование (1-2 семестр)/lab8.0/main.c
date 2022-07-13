#pragma warning(disable : 4996)
#include <stdio.h>
#include <malloc.h>
#include <limits.h>

//Структура вершины
typedef struct edge
{
	int size;
	int from;
	int to;
}edge;

//Проверяем данные
int input_edges(edge* graph, int N, int M)
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
		graph[i].size = size;
		graph[i].from = from - 1;
		graph[i].to = to - 1;
	}
	return 0;
}

//Алгоритм сортировки Heap-Sort
void swap(edge* a, edge* b)
{
	edge c = *a;
	*a = *b;
	*b = c;
}
void heap_sort(edge* graph, int i, int max_id)
{
	int large_id = i;
	int left_id = 2 * i + 1;
	int right_id = 2 * i + 2;
	if ((left_id < max_id) && (graph[left_id].size > graph[large_id].size)/* && (graph[left_id].size > graph[right_id].size)*/)
	{
		large_id = left_id;
	}
	if ((right_id < max_id) && (graph[right_id].size> graph[large_id].size)/* && (graph[right_id].size> graph[left_id].size)*/)
	{
		large_id = right_id;
	}
	if ((large_id < max_id) && (large_id != i))
	{
		swap(&graph[i], &graph[large_id]);
		heap_sort(graph, large_id, max_id);
	}
}
void start_sort(edge* graph, int M)
{
	for (int i = M / 2 - 1; i >= 0; i--)
	{
		heap_sort(graph, i, M);
	}
	for (int i = M - 1; i > 0; i--)
	{			
		if (graph[0].size > graph[i].size)
		{
			swap(&graph[0], &graph[i]);
		}
		heap_sort(graph, 0, i);
	}
}

//Заполнение массивов
void create_massives(short* parent, short* rank, int N)
{
	for (int i = 0; i < N; i++)
	{
		parent[i] = i;
		rank[i] = 1;
	}
}

//Вспомогательная функция поиска parent
int find(int i, short* parent)
{
	if (parent[i] == i)
	{
		return i;
	}
	return parent[i] = find(parent[i], parent);
}

//Приравниваем значение parent
void merge(int from, int to, short* rank, short* parent)
{
	if (rank[from] < rank[to])
	{
		parent[from] = to;
	}
	else
	{
		parent[to] = from;
		if (rank[from] == rank[to])
		{
			rank[from]++;
		}
	}
}

//Алгоритм Краскала
int algoritm_Kruskal(edge* graph, short* parent, short* rank, int* print_edge, int M, int N)
{
	create_massives(parent, rank, N);
	int count = 0; 
	for (int i = 0; i < M; i++)
	{
		int from = find(graph[i].from, parent);
		int to = find(graph[i].to, parent);
		if (from != to)
		{
			merge(from, to, rank, parent);
			print_edge[count] = i;
			count++;
		}
	}
	if (count != N - 1)
	{
		return 6;
	}
	return 0;
}

//Выводим ответ
void print(edge* graph, int* print_edge, int N)
{
	for (int i = 0; i < N - 1; i++)
	{
		printf("%d %d\n", (graph[print_edge[i]].from + 1), (graph[print_edge[i]].to + 1));
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

	edge* graph = (edge*)malloc((M + 1) * sizeof(edge));
	short* parent = (short*)malloc(N * sizeof(short));
	short* rank = (short*)malloc(N * sizeof(short));
	int* print_edge = (int*)malloc(N * sizeof(int));

	//Заполняем и проверяем данные
	error_code = input_edges(graph, N, M);
	if (error_code == 0)
	{
		start_sort(graph, M); 
		error_code = algoritm_Kruskal(graph, parent, rank, print_edge, M, N);
	}

	switch (error_code)
	{
		case 0: print(graph, print_edge, N); break;
		case 1: printf("bad number of vertices"); break;
		case 2: printf("bad number of edges"); break;
		case 3: printf("bad vertex"); break;
		case 4: printf("bad length"); break;
		case 5: printf("bad number of lines"); break;
		case 6: printf("no spanning tree"); break;
	}

	free(graph);
	free(parent);
	free(rank);
	free(print_edge);

	return 0;
}
