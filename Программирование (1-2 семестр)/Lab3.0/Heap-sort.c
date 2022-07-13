//#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)
#include <stdio.h>
#include <malloc.h>

void swap(int* a, int* b)
{
	int c = *a;
	*a = *b;
	*b = c;
}

//Сортируем кучку
void heap_sort(int* a, int i, int max_id)
{
	int large_id = i;
	int left_id = 2 * i + 1;
	int right_id = 2 * i + 2;
	if ((left_id < max_id) && (a[left_id] > a[large_id]) && (a[left_id] > a[right_id]))
	{
		large_id = left_id;
	}
	if ((right_id < max_id) && (a[right_id] > a[large_id]) && (a[right_id] > a[left_id]))
	{
		large_id = right_id;
	}
	if ((large_id < max_id) && (large_id != i))
	{
		swap(&a[i], &a[large_id]);
		heap_sort(a, large_id, max_id);
	}
}

int main()
{
	//Вводим количество элементов
	int N;
	if (scanf("%d", &N) != 1)
	{
		return 0;
	}
	//Вводим массив
	int* a = (int*)malloc((N + 1) * sizeof(int));
	for (int i = 0; i < N; i++)
	{
		if (scanf("%d", &a[i]) != 1)
		{
			return 0;
		}
	}
	//Делаем первую сортировку
	for (int i = N / 2 - 1; i >= 0; i--)
	{
		heap_sort(a, i, N);
	}
	//Делаем сортировку с отбрасыванием последних
	for (int j = N - 1; j > 0; j--)
	{			//Меняем последнее с первым
		if (a[0] > a[j])
		{
			swap(&a[0], &a[j]);
		}
		heap_sort(a, 0, j);
	}
	//Выводим ответ
	for (int i = 0; i < N; i++)
	{
		printf("%d ", a[i]);
	}
	free(a);
	return 0;
}
