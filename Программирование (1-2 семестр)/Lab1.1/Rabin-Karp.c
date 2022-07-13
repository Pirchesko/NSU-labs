#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//Вычисление хеша
int Hash(char* pattern)
{
    int hash = 0;
    int dlina = strlen(pattern);
    for (int i = 0; i < dlina; i++)
    {
        hash += ((unsigned char)pattern[i] % 3) * pow(3, i);
    }
    return hash;
}

//Создание шаблона для вычисления хеша
void Mobile_Pattern(char* text, char* mobile_pattern, int d_pattern, int k_text)
{
    for (int j = 0; j < d_pattern; j++)
    {
        mobile_pattern[j] = text[j + k_text];
    }
}

//Поиск строки в подстроки
void Algoritm_Rabin_Karp(char* text, char* pattern)
{
    int d_text = strlen(text); //длина текста
    int d_pattern = strlen(pattern); //длина строки
    int hash_pattern = Hash(pattern); //сначала выведем значение хеша
    printf("%d ", hash_pattern);
    char mobile_pattern[17] = { 0 }; //создание подвижного шаблона для вычисления хеша
    for (int i = 0; i < d_text - d_pattern + 1; i++)
    {
        Mobile_Pattern(text, mobile_pattern, d_pattern, i); //создание подвижного шаблона
        int hash_mobile_pattern = Hash(mobile_pattern); //хеш подвижного шаблона
        //Проверяем хеши
        if (hash_mobile_pattern == hash_pattern)//и если хеши равны, реализуем проверку посимвольно
        {
            for (int j = 0; j < d_pattern; j++)
            {
                printf("%d ", (j + i + 1));
                if (pattern[j] != mobile_pattern[j])
                {
                    break;
                }
            }
        }
    }
}

int main()
{
    //Считывание шаблона
    char pattern[18] = { 0 }; //шаблон
    if (fgets(pattern, 18, stdin) == NULL)
    {
        return 0;
    }
    int d_pattern = strlen(pattern);
    pattern[d_pattern - 1] = '\0';

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

    //Запускаем алгоритм
    Algoritm_Rabin_Karp(text, pattern);

	return 0;
}
