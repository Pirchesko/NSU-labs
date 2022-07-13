//#define _CRT_SECURE_NO_WARNINGS
//#pragma warning(disable : 6387)
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

//Создание шаблона
void Create_stop_symbols(int* stop_symbols, char* pattern) {
    int d_pattern = strlen(pattern) - 1;
    int k = 1;
    //Заполняем массив с пердпоследнего элемента
    for (int i = d_pattern - 2; i >= 0; i--)
    {
        if (stop_symbols[(int)pattern[i]] == 0)
        {
            stop_symbols[(int)pattern[i]] = k;
        }
        k++;
    }
    //Заполняем последний элемент шаблона
    if (stop_symbols[(int)pattern[d_pattern - 1]] == 0)
    {
        stop_symbols[(int)pattern[d_pattern - 1]] = k;
    }
    //Заполняем оставшиеся место
    for (int i = 0; i < 256; i++)
    {
        if (stop_symbols[i] == 0)
        {
            stop_symbols[i] = d_pattern;
        }
    }
}

//Поиск подстроки в строке
void Algoritm_Boyer_Moore(char* text, char* pattern, int* stop_symbols) {
    int d_text = strlen(text); //длина текста
    int d_pattern = strlen(pattern) - 1; //длина шаблона
    int k_pattern = d_pattern - 1; //подвижная длина шаблона (координата шаблона)
    while (k_pattern < d_text) //шаблон не должен выйти за перделы текста
    {
        int k1_pattern = d_pattern - 1; //координата шаблона на каждый новый сдвиг
        int k_text = k_pattern; //координата текста
        while ((k1_pattern > 0) && (pattern[k1_pattern] == text[k_text])) //сравниваем
        {
            printf("%d ", k_text + 1); //выврдим на 1 больше т.к. это позиция, а не индекс
            k_text--;
            k1_pattern--;
        }
        printf("%d ", k_text + 1); //выврдим на 1 больше т.к. это позиция, а не индекс
        k_pattern += stop_symbols[(int)text[k_pattern]]; //сдвигаем шаблон
    }
}

int main()
{
    int stop_symbols[256] = { 0 }; //масиив стоп символов

    //Считывание шаблона
    char pattern[20] = { 0 }; //шаблон
    if (fgets(pattern, 20, stdin) == NULL)
    {
        return 0;
    } 

    //Считывание текста
    char text[100] = { 0 }; //текст
    int i = 0;
    text[i] = getchar();
    while (text[i] != EOF)
    {
        i++;
        text[i] = getchar();
    }
    text[i] = '\0';
    
    //Начало алгоритма
    Create_stop_symbols(stop_symbols, pattern); //создаём таблицу стоп-символов
    Algoritm_Boyer_Moore(text, pattern, stop_symbols); //реализуем алгоритм Бора-Мура
    return 0;
}
