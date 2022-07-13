#pragma warning(disable : 4996)
#include <stdio.h>
#include <malloc.h>

//Создание списка рёбер
typedef struct ribs
{
    int from; 
    int to;
    struct list* next_rib; 
} ribs;

//Создание списка смежностей
typedef struct list
{
    int data; 
    struct list* next;
} list;

//Считаем сколько списков смежностей создавать
int number_of_lists(struct ribs* input)
{
    int max = 0;
    while (input != NULL)
    {
        if (max < input->from)
        {
            max = input->from;
        }
        if (max < input->to)
        {
            max = input->to;
        }
        input = input->next_rib;
    }
    return max;
}

//Добавление элемента в список смежностей
void add_list(struct list** vershina, int to)
{
    list* a = (list*)malloc(sizeof(list));
    a->data = to;
    a->next = (*vershina);
    (*vershina) = a;
}

//while (input != NULL)
int check_input(struct ribs* input, int check)
{
    int i = 0;
    while ((input != NULL) && (i < check))
    {
        input = input->next_rib;
        i++;
    }
    if (input == NULL)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

//(input)->from
int from_input(struct ribs* input, int check)
{
    int i = 0;
    while ((input != NULL) && (i < check))
    {
        input = input->next_rib;
        i++;
    }
    return input->from;
}

//(input)->to
int to_input(struct ribs* input, int check)
{
    int i = 0;
    while ((input != NULL) && (i < check))
    {
        input = input->next_rib;
        i++;
    }
    return input->to;
}

//Вывод списка смежностей
void print_list(struct list* vershina, FILE* out)
{
    while (vershina != NULL)
    {
        fprintf(out, "%2d ", vershina->data);
        vershina = vershina->next;
    }
}

//Вытаскиваем цифру
int extract_number(char* text, int* i)
{
    int number = 0;
    while ((text[(*i)] != ' ') && (text[(*i)] != '\n'))
    {
        number *= 10;
        number += (int)text[(*i)] - (int)'0';
        (*i)++;
    }
    return number;
}

//Корректируем ввод
void create_ribs(struct ribs** input)
{
    FILE* in;
    in = fopen("D:\\Files\\Visual Studio\\Ribs_to_List\\Debug\\in.txt", "r");
    int from;
    int to;
    while (fscanf(in, "%d %d", &from, &to) > 0)
    {
        ribs* a = (ribs*)malloc(sizeof(ribs));
        a->from = from;
        a->to = to;
        a->next_rib = (*input);
        (*input) = a;
    }
    fclose(in);
}

int main()
{
    //Поддрежка русского языка в консоли
    system("chcp 1251");
    system("cls");

    //Создаём список рёбер
    ribs* input = NULL;
    create_ribs(&input);

    //Создаём массив списков смежностей
    int N = number_of_lists(input);
    list** list_vershin = (list**)malloc((N + 1) * sizeof(list*));
    for (int i = 0; i <= N; i++)
    {
        list_vershin[i] = NULL;
    }

    //Заполняем список смежностей
    int check = 0;
    int from;
    int to;
    while (check_input(input, check) != 0)
    {
        from = from_input(input, check);
        to = to_input(input, check);
        add_list(&list_vershin[from], to);
        check++;
    }

    //Выводим спискок смежностей
    FILE* out;
    out = fopen("D:\\Files\\Visual Studio\\homework2\\Debug\\out.txt", "w");
    for (int i = 0; i <= N; i++)
    {
        if (list_vershin[i] != NULL)
        {
            fprintf(out, "%2d | ", i);
            print_list(list_vershin[i], out);
            fprintf(out, "\n");
        }

    }
    fclose(out);

    //Чистим память
    for (int i = 0; i <= N; i++)
    {
        list* it1 = list_vershin[i];
        while (it1)
        {
            list* next = it1->next;
            free(it1);
            it1 = next;
        }
    }
    free(list_vershin);
    ribs* it2 = input;
    while (it2)
    {
        ribs* next = it2->next_rib;
        free(it2);
        it2 = next;
    }
    printf("\n");
    return 0;
}

/*
    Примеры:
    Граф А:
    0 1
    0 2
    0 6
    1 0
    1 2
    1 3
    2 0
    2 1
    3 1
    3 4
    4 3
    4 5
    5 4
    5 6
    6 0
    6 5

    Граф B:
    0 1
    0 3
    1 0
    1 2
    1 7
    2 1
    2 3
    2 8
    3 0
    3 2
    3 4
    4 3
    4 5
    4 6
    5 4
    5 9
    6 4
    7 1
    7 8
    8 2
    8 7
    8 9
    9 5
    9 8
    9 10
    10 9

    Граф С:
    0 9
    1 3
    2 5
    2 7
    3 1
    3 4
    3 6
    3 8
    4 3
    5 2
    5 10
    5 12
    6 3
    6 8
    6 13
    7 2
    8 3
    8 6
    8 9
    9 0
    9 8
    9 11
    10 5
    10 12
    11 9
    11 13
    12 5
    12 10
    13 6
    13 11

    Граф Д:
    0 2
    0 3
    1 0
    1 4
    2 1
    3 6
    4 5
    5 2
    5 4
    5 6
    6 2
    6 3

    Граф Е:
    0 2
    0 3
    2 1
    2 5
    3 0
    4 1
    4 5
    5 2
    5 4
    6 2
    6 3

    Граф Ф:
    0 9
    2 5
    3 1
    3 4
    3 6
    3 8
    4 3
    4 12
    5 2
    5 12
    6 3
    6 13
    7 2
    8 6
    8 9
    9 0
    9 11
    10 5
    11 9
    11 13
    12 10
    13 6
*/