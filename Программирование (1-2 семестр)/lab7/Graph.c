#pragma warning(disable : 4996)
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

typedef struct stack
{
	int* data;
	int size;
}stack;

stack push(stack* sorted, int value)
{
	sorted->size++;
	sorted->data[sorted->size] = value;
	return *sorted;
}

void print(stack* sorted)
{
	for (int i = sorted->size; i >= 0; i--)
	{
		printf("%d ", sorted->data[i]);
	}
}

int check_input(int N, int M, int* graph, int* used, int from, int to)
{
	memset(graph, 0, (N * N + 1));
	for (int i = 0; i <= N; i++)
	{
		used[i] = 0;
	}
	//Вводим остальные данные и проверяем их
	for (int i = 1; i <= M; i++)
	{
		if (scanf("%d %d", &from, &to) != EOF)
		{
			if ((to < 1) || (to > N) || (from < 1) || (from > N))
			{
				return 4;
			}
		}
		else
		{
			if (N == 0)
			{
				return 3;
			}
			else
			{
				return 1;
			}
		}
		graph[(from - 1) * N + (to - 1)] = 1;
	}
	if (((from == -1) || (to == -1)) && (M != 0))
	{
		return 1;
	}
	else if ((N < 0) || (N > 1000))
	{
		return 2;
	}
	else if ((M > (N * (N - 1) / 2)) || (M < 0))
	{
		return 3;
	}
	return 0;
}

int DFS(stack* sorted, int* graph, int* used, int N, int vershina)
{
	if (used[vershina] == 2)
	{
		return 0;//!!!
	}
	else if (used[vershina] == 1)
	{
		return 5;
	}
	else
	{
		used[vershina] = 1;
		for (int i = 1; i <= N; i++)
		{
			if (graph[(vershina - 1) * N + (i - 1)] == 1)
			{
				if (DFS(sorted, graph, used, N, i) == 5)
				{
					return 5;
				}
			}
		}
		used[vershina] = 2;
		push(sorted, vershina);
		return 0;
	}
}

int topological_sort(stack* sorted, int* graph, int* used, int N)
{
	for (int i = 1; i <= N; i++)
	{
		if (used[i] == 0)
		{
			if (DFS(sorted, graph, used, N, i) == 5)
			{
				return 5;
			}
		}
	}
	return 0;
}

int main()
{
	//Переменные								//0 - успешно
	int N;										//1 - "bad number of lines"
	int M;										//2 - "bad number of vertices"
	int from = -1;								//3 - "bad number of edges"
	int to = -1;								//4 - "bad vertex"
	int error_code = -1; //код возврата данных: //5 - "impossible to sort"

	//Ввод N и M
	if (scanf("%d %d", &N, &M) != 2)
	{
		printf("bad number of lines");
		return 0;
	}
	
	//Выделяем память
	int* graph = (int*)malloc((N * N + 1) * sizeof(int)); //создаём матрицу смежности
	int* used = (int*)malloc((N + 1) * sizeof(int)); //посещённые или непосещённые вершины
	stack sorted; //стэк для отсортировки
	sorted.data = (int*)malloc((N + 1) * sizeof(int));
	sorted.size = -1;

	//Проверяем входные данные
	error_code = check_input(N, M, graph, used, from, to); 

	//Запускаем топологическую сортировку
	if (error_code == 0) 
	{
		error_code = topological_sort(&sorted, graph, used, N);
	}

	//Выводим ответ
	switch (error_code)
	{
		case 0: print(&sorted); break;
		case 1: printf("bad number of lines"); break;
		case 2: printf("bad number of vertices"); break;
		case 3: printf("bad number of edges"); break;
		case 4: printf("bad vertex"); break;
		case 5: printf("impossible to sort"); break;
	}

	//Чистим память
	free(sorted.data);
	free(used);
	free(graph);
	return 0;
}
