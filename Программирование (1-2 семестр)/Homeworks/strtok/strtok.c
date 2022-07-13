#include <stdio.h>
#include <string.h>

int i = 0; //счётчик
int d_str = -1; //длина строки
int d_sep = -1; //длина лексемы
int j = 0; //откуда будет вывод str

void check_double_sep(char* str, char* sep)
{
	//Эта функция для того, чтобы при повторе лексем лишних "ентеров" при выводе не было
	for (int k = 0; k < d_sep; k++)
	{
		if (str[i] == sep[k])
		{
			i++;
			check_double_sep(str, sep);
		}
	}
}

char* mystrtok(char* str, char* sep)
{
	if ((d_str == -1) || (d_sep == -1))	//сохраним длины
	{
		d_str = strlen(str);
		d_sep = strlen(sep);
	}
	check_double_sep(str, sep); //сделаем проверочку на повторы лексем
	j = i; //каждый раз новый вывод
	int i1 = 99999; //т.к. возможен выбор из нескольких лексем координата i может быть разная, найдём минимальную её
	for (int k = 0; k < d_sep; k++)
	{
		while (str[i] != sep[k]) //ищем координату с которой начианется следующая лексема
		{
			if (i >= d_str) //координата не должна выйти за границы строки
			{
				break;
			}
			i++;
		}
		if (i < i1) //ищем минимальную i
		{
			i1 = i;
		}
		i = j; //т.к. операцию делаем несколько раз, возращаем изначальное значение i
	}
	i = i1;
	if (i <= d_str) //ограничиваем строку до лексемы
	{
		str[i] = '\0';
		i++;
	}
	else //если уже конец строки
	{
		return NULL;
	}
	return (str + j); //делаем +j чтобы вывод начинался с нужной лексемы
}

int main(void)
{
	// Массив со строкой для поиска
	char str[100];
	// Набор символов, которые должны входить в искомый сегмент
	char sep[100];
	// Переменная, в которую будут заноситься начальные адреса частей
	char* istr;

	puts("Stroka: ");
	gets(str);
	puts("Lexema: ");
	gets(sep);

	istr = mystrtok(str, sep);
	while (istr != NULL)
	{
		printf("%s\n", istr);
		istr = mystrtok(str, sep);
	}
	return 0;
}