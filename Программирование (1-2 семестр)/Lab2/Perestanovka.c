//#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//Ошибка
void error()
{
	printf("bad input");
	exit(0);
}

//Проверка на правильность ввода шаблона
void check_input(const char* ch, int d, int p)
{
	//Проверка на корректные символы
	for (int i = 0; i < d; i++)
	{
		if ((ch[i] < '0') || (ch[i] > '9'))
		{
			error();
		}
	}
	//Не может быть меньше единицы
	if (p < 1)
	{
		exit(0);
	}
	//Символы не должны повторяться
	for (int i = 0; i < d - 1; i++)
	{
		for (int j = i + 1; j < d; j++)
		{
			if (ch[i] == ch[j])
			{
				error();
			}
		}
	}
}

//Преобразуем строку в массив int
void char_to_int(const char* ch, int* a, int d)
{
	for (int i = 0; i < d; i++)
	{
		a[i] = (int)ch[i] - 48;
	}
}

//Проверим возможна ли что-то сделать с массивом
void check_massive(const int* a, int d)
{
	int check = 0;
	for (int i = d - 2; i >= 0; i--)
	{
		if (a[i] < a[i + 1])
		{
			check = 1;
			break;
		}
	}
	if (check == 0)
	{
		exit(0);
	}
}

//Поиск ниабольшего j такого, что a[j] < a[j + 1]
int find_J(const int* a, int d)
{
	for (int i = d - 1; i > 0; i--)
	{
		if (a[i] < a[i + 1])
		{
			return i;
		}
	}
	return 0;
}

//Поиск наибольшего l, такого что l > j, для которого a[l] > a[j]
int find_L(const int* a, int d, int j)
{
	for (int i = d - 1; i > 0; i--)
	{
		if ((i > j) && (a[i] > a[j]))
		{
			return i;
		}
	}
	return 0;
}

//Меняем местами a[j] и a[l]
void swap(int* a, int j, int l)
{
	int t = a[j];
	a[j] = a[l];
	a[l] = t;
}

//Записываем в обратном порядке a[j+1]..a[n]
void obratno(int* a, int j, int d)
{
	int j1 = j + 1;
	//Так как не знаем с какой координаты начинается замена придётся делать так
	int c = (d - j1) / 2;
	int k = 1;
	for (int i = j1; c > 0; i++)
	{
		int t;
		t = a[i];
		a[i] = a[d - k];
		a[d - k] = t;
		c--;
		k++;
	}
}

//Выводим ответ
void print(int* a, int d)
{
	for (int i = 0; i < d; i++)
	{
		printf("%d", a[i]);
	}
}

int main()
{
	//Переменные
	char ch[11] = { 0 }; //строка содержащая цифры
	int d; //длинна строки
	int p; //количество перестановок
	int a[11] = { 0 }; //массив для преобразования строки в массив

	//Считываем данные
	if (scanf("%10s", ch) != 1) //строка для перестановок
	{
		return 0;
	}
	if (scanf("%d", &p) != 1) //цифра P
	{
		return 0;
	}

	d = strlen(ch); //длинна строки
	check_input(ch, d, p); //проверяем правильность шаблона
	char_to_int(ch, a, d); //преобразуем строку в массив int

	while (p > 0)
	{
		check_massive(a, d); //проверяем возможность что-то делать с массивом
		int j = find_J(a, d); //ищем ниабольший j, такой что a[j] < a[j + 1]
		int l = find_L(a, d, j); //ищем наибольшее l, такое что l > j, для которого a[l] > a[j]
		swap(a, j, l); //меняем местами a[j] и a[l]
		obratno(a, j, d); //записываем в обратном порядке a[j + 1]..a[n]
		print(a, d); //выводим ответ (если нужно)
		printf("\n");
		p--;
	}
	return 0;
}
