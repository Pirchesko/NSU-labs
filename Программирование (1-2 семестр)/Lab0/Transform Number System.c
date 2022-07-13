#define _CRT_SECURE_NO_WARNINGS
//#pragma warning(disable : 4996)
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void error()
{
	printf("bad input");
	exit(0);
}

//Функция для представления цифры в формате int
long long numtoint(long long num)
{
	if ((num >= 48) && (num <= 57))
	{
		return (num - 48);
	}
	else if ((num >= 65) && (num <= 70))
	{
		return (num - 55);
	}
	else if ((num >= 97) && (num <= 102))
	{
		return (num - 87);
	}
	else if ((num == 44) || (num == 46))
	{
		num = -1;
		return num;
	}
	else
	{
		error();
	}
	return 0;
}

//Функция для представления цифры в формате char
long long numtochar(long long num)
{
	if ((num >= 0) && (num <= 9))
	{
		return (num + 48);
	}
	else if ((num >= 10) && (num <= 16))
	{
		return (num + 55);
	}
	else
	{
		error();
	}
	return 0;
}

int main()
{
	//p - Текущая СС
	//q - Новая СС
	//ch - Текущее число
	int p, q;
	int input;
	input = scanf("%d %d", &p, &q);
	if (input != 2)
	{
		return 0;
	}
	char ch[14] = { 0 };
	input = scanf("%13s", ch);
	if (input != 1)
	{
		return 0;
	}

	//Допонительные переменные
	int k;
	int c;
	int lenth = strlen(ch);

	//Проверка на bad input
	if (p < 2 || q < 2 || q > 16 || p > 16) //СС должна быть от 2-ричной до 16-ричной
	{
		error();
	}
	if ((ch[0] == ',') || (ch[0] == '.') || (ch[lenth - 1] == ',') || (ch[lenth - 1] == '.')) //Число не может начинаться и заканчиваться точкой или запятой
	{
		error();
	}
	long long n; //В числе цифры не превышают СС
	c = 0;//Посчитаем количество циферок
	int tochka = -1; //Координата точки
	int ktochka = 0; //Проверка на количество точек
	for (int i = 0; i < lenth; i++)
	{
		n = numtoint(ch[i]);
		if (n == -1)
		{
			tochka = i;
			ktochka += 1;
		}
		if ((n >= p) && (n != -1))
		{
			error();
		}
		c++;
	}
	if (ktochka > 1) //Точек не может быть больше 2-ух
	{
		error();
	}
	if ((ch[0] == '0') && (ch[1] != ',') && (ch[1] != '.') && (c > 1))//Число не может начинаться с 0 (кроме случаев с дробной частью)
	{
		error();
	}

	//Берём координату точки
	int xt = tochka;
	if (tochka == -1)
	{
		xt = lenth;
	}

	//Строим таблицу степеней для перевода в новую СС
	long long st[13];
	st[0] = 1;
	for (int i = 1; i < 13; i++)
	{
		st[i] = st[i - 1] * p;
	}

	//ch10 - Переводим целую часть в 10-ричную СС
	long long ch10 = 0;
	k = 0;//Координата
	for (int i = xt - 1; i >= 0; i--)//i - степень
	{
		ch10 += numtoint(ch[k]) * st[i];
		k++;
	}

	//ch10t - Переводим дробную часть в 10-ричную СС
	long long ch10t = 0;
	long long qdel = 0; //qdel - Делитель для перевода дробной части в новую СС
	//int numb;
	if ((tochka != -1) && (xt != lenth))
	{
		qdel = q * st[lenth - xt - 1];
		k = lenth - xt - 2;//Степень
		for (int i = xt + 1; i < lenth; i++)//i - координата
		{
			ch10t += numtoint(ch[i]) * st[k];
			k--;
		}
	}

	//Считаем количество цифр для новой СС
	c = 0;
	long long ch10n = ch10;
	if (ch10n == 0)
	{
		c++;
	}
	while (ch10n > 0)
	{
		c++;
		ch10n /= q;
	}

	//Переводим в новую СС (целую часть)
	char newch[64] = { 0 };
	int c1 = c;//Координата
	ch10n = ch10;
	while (c1 > 0)
	{
		n = ch10n % q;
		newch[c1 - 1] = numtochar(n);
		ch10n /= q;
		c1--;
	}

	//Переводим в новую СС (дробную часть)
	k = 0;//Ограничиваем 12 знакакми после запятой
	int c2 = c;//Координата
	if ((tochka != -1) && (ch10t != 0))
	{
		ch10t = (ch10t * q) - (qdel * ((ch10t * q) / qdel)); //Первый прогон будет всегда 0, поэтому мы его пропустим
		while ((ch10t > 0) && (k < 12))
		{
			n = (ch10t * q) / qdel;
			newch[c2 + 1] = numtochar(n);
			c2++;
			k++;
			ch10t = (ch10t * q) - (qdel * n);
		}
	}
	else
	{
		tochka = -1;
	}

	//Плоготавливаем ответ для вывода
	if (tochka != -1)
	{
		k = c2 + 1;
		newch[c] = '.';
	}
	else
	{
		k = c - 1;
	}
	//Выводим ответ
	for (int i = 0; i <= k; i++)
	{
		printf("%c", newch[i]);
	}

	return 0;
}
