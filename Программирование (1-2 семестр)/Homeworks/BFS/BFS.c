#pragma warning(disable : 4996)
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>

//Создание очереди 
typedef struct queue
{    
    int queue[100];
    int end; //позиция последнего элемента в очереди
    int begin; //позиция первого элемента в очереди
} queue;

//Инициализация очереди
void init(struct queue* q)
{
    q->begin = 1;
    q->end = 0;
    return;
}

//Проверка пуста ли очередь
int empty(struct queue* q)
{
    if (q->end < q->begin)
    {
        return -1; //пусто
    }
    else
    {
        return 1; //есть элементы
    }
}

//Проверка очереди для более глубоких случаев
int is_empty(struct queue* q, const int** matrix, const int size, int vershina, int* status)
{
    if (empty(q) == 1)
    {
        return 1;
    }
    else
    {
        int check = 0;
        for (int i = 0; i < size; i++)
        {
            if ((matrix[vershina][i] == 1) && (status[i] == 0))
            {
                check = 1;
            }
        }
        if (check == 1)
        {
            return 1;
        }
        else
        {
            return -1;
        }
    }
}

//Добавление элемента в очередь
void add(struct queue* q, int value)
{
    q->end++;
    q->queue[q->end] = value;
}

//Удаление элемента из очереди
int delete(struct queue* q)
{
    int value = q->queue[q->begin];
    q->begin++;
    return value;
}

//Вывод списка
void print_queue(struct queue* q)
{
    int h;
    printf("Очередь: ");
    for (h = q->begin; h <= q->end; h++)
    {
        printf("%d ", q->queue[h]);
    }
    printf("\n\n");
    return;
}

//Вывод статуса вершин
void print_status(const int* status, const int size)
{
    printf("Статус вершин: ");
    for (int i = 0; i < size; i++)
    {
        printf("%d |'%d',  ", i, status[i]); //то что до | это вершина, а в '' это статус
    }
    printf("\n");
}

//Вывод матрицы смежности
void print_matrix(const int** matrix, const int size)
{
    printf("Матрица смежности: \n");
    for (int i = 0; i < size; i++) 
    {
        for (int j = 0; j < size; j++)
        {
            printf("%2d ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

//Выводим расстояния между вершинами
void print_distance(const int* distance, const int size)
{
    int max_distance = 0;
    printf("\nВершина | Расстояние\n");
    for (int i = 0; i < size; i++)
    {
        printf("%d | %d\n", i, distance[i]);
        if (max_distance < distance[i])
        {
            max_distance = distance[i];
        }
    }
    printf("\nРасстояние | Вершина\n");
    for (int i = 0; i <= max_distance; i++)
    {
        printf("%d | ", i);
        for (int j = 0; j < size; j++)
        {
            if (distance[j] == i)
            {
                printf("%d ", j);
            }
        }
        printf("\n");
    }
}

//Корректируем введённые символы
void fix_input(char* text)
{
    int i = 0;
    int j = 0;
    char fix[1000];
    while ((text[i] != '\n') && (text[i] != '\0'))
    {
        if ((text[i] == '0') || (text[i] == '1'))
        {
            fix[j] = text[i];
            i++;
            j++;
        }
        else
        {
            i++;
        }
    }
    fix[j] = '\0';
    for (int k = 0; k < strlen(fix); k++)
    {
        text[k] = fix[k];
    }
    text[strlen(fix)] = '\0';
}

//Создаём двумерный массив
void create_matrix(int **matrix, const char* text, const int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            matrix[i][j] = text[i * size + j] - 48;
        }
    }
}

//Считаем расстояние между вершинами
void check_distance(const int **matrix, const int size, int* distance, int vershina, int* status)
{
    for (int i = 0; i < size; i++)
    {
        if ((matrix[i][vershina] == 1) && (vershina != i))
        {
            if (distance[i] != -1)
            {
                if (distance[vershina] == -1)
                {
                    distance[vershina] = distance[i] + 1;
                }
                else if (distance[i] < distance[vershina])
                {
                    distance[vershina] = distance[i] + 1;
                }
            }
        }
    }
    if (distance[vershina] == -1)
    {
        distance[vershina] = 0;
    }
}

//Алгоритм BFS с подсчётом расстояний
void bfs(const int** matrix, const int size, int vershina)
{
    //Создание очереди 
    queue* q;
    q = (queue*)malloc(sizeof(queue));
    init(q);

    //Посещённые вершины
    int* status = (int*)malloc(size * sizeof(int)); //0 - не посещённые, 1 - уже в списке есть, 2 - посещённые
    for (int i = 0; i < size; i++)
    {
        status[i] = 0;
    }

    //Расстояния
    int* distance = (int*)malloc(size * sizeof(int));
    for (int i = 0; i < size; i++)
    {
        distance[i] = -1;
    }

    status[vershina] = 1; //рассматриваем вершину 0
    distance[vershina] = 0;
    print_status(status, size);
    do
    {
        //Пробегаемся по рёбрам
        for (int i = 0; i < size; i++)
        {
            //По матрице смежности находим следующие вершины которые нужно рассмотреть
            if ((matrix[vershina][i] == 1) && (status[i] == 0))
            {
                add(q, i); //и добавляем их в очередь
                status[i] = 1; //статус "в очереди"
                distance[i] = distance[vershina] + 1;
            }
        }
        if (empty(q) == -1)
        {
            break;
        }
        print_queue(q); //выводим очередь
        status[vershina] = 2; //текущая вершина становится посещённой
        vershina = delete(q); //удаляем её из очереди
        print_status(status, size);//выводим статус
        //check_distance(matrix, size, distance, vershina, status); //считаем рассотяния
    } while (is_empty(q, matrix, size, vershina, status) != -1); //пока очередь не пуста
    print_distance(distance, size);
}

int main()
{
    //Поддрежка русского языка в консоли
    system("chcp 1251");
    system("cls");
    
    //Считываем матрице смежности в произвольном виде и корректируем её для вставки в двумерный массив
    char text[1000];
    fgets(text, 1000, stdin);
    fix_input(text);

    //Выбираем вершину откуда будем считать
    int vershina;
    printf("Вершина: ");
    if (scanf("%d", &vershina) != 1)
    {
        return 0;
    }
    printf("\n");

    //Создаём динамический массив для матрицы смежности
    int size = sqrt(strlen(text));
    int** matrix = (int**)calloc(size, sizeof(int*));
    for (int i = 0; i < size; i++)
    {
        matrix[i] = (int*)calloc(size, sizeof(int));
    }
    
    //Создаём и выводим матрицу смежности
    create_matrix(matrix, text, size);
    print_matrix(matrix, size);

    printf("'0' - не посещённые\n");
    printf("'1' - в очереди\n");
    printf("'2' - посещённые\n\n");

    //Запускаем алгоритм BFS
    bfs(matrix, size, vershina);

    free(matrix);
    return 0;
}

/*
    Граф А:
    0 1 1 0 0 0 1
    1 0 1 1 0 0 0
    1 1 0 0 0 0 0
    0 1 0 0 1 0 0
    0 0 0 1 0 1 0
    0 0 0 0 1 0 1
    1 0 0 0 0 1 0

    Граф Б:
    0 1 0 1 0 0 0 0 0 0 0
    1 0 1 0 0 0 0 1 0 0 0
    0 1 0 1 0 0 0 0 1 0 0
    1 0 1 0 1 0 0 0 0 0 0
    0 0 0 1 0 1 1 0 0 0 0
    0 0 0 0 1 0 0 0 0 1 0
    0 0 0 0 1 0 0 0 0 0 0
    0 1 0 0 0 0 0 0 1 0 0
    0 0 1 0 0 0 0 1 0 1 0
    0 0 0 0 0 1 0 0 1 0 1
    0 0 0 0 0 0 0 0 0 1 0

    Граф С:
    0 0 0 0 0 0 0 0 0 1 0 0 0 0 
    0 0 0 1 0 0 0 0 0 0 0 0 0 0
    0 0 0 0 0 1 0 1 0 0 0 0 0 0
    0 1 0 0 1 0 1 0 1 0 0 0 0 0
    0 0 0 1 0 0 0 0 0 0 0 0 0 0
    0 0 1 0 0 0 0 0 0 0 1 0 1 0
    0 0 0 1 0 0 0 0 1 0 0 0 0 1
    0 0 1 0 0 0 0 0 0 0 0 0 0 0
    0 0 0 1 0 0 1 0 0 1 0 0 0 0
    1 0 0 0 0 0 0 0 1 0 0 1 0 0
    0 0 0 0 0 1 0 0 0 0 0 0 1 0
    0 0 0 0 0 0 0 0 0 1 0 0 0 1
    0 0 0 0 0 1 0 0 0 0 1 0 0 0
    0 0 0 0 0 0 1 0 0 0 0 1 0 0

    Граф Д:
    0 0 1 1 0 0 0
    1 0 0 0 1 0 0
    0 1 0 0 0 0 0
    0 0 0 0 0 0 1
    0 0 0 0 0 1 0
    0 0 1 0 1 0 1
    0 0 1 1 0 0 0

    Граф Е:
    0 0 1 1 0 0 0
    0 0 0 0 0 0 0
    0 1 0 0 0 1 0
    1 0 0 0 0 0 0
    0 1 0 0 0 1 0
    0 0 1 0 1 0 0
    0 0 1 1 0 0 0

    Граф С:
    0 0 0 0 0 0 0 0 0 1 0 0 0 0
    0 0 0 0 0 0 0 0 0 0 0 0 0 0
    0 0 0 0 0 1 0 0 0 0 0 0 0 0
    0 1 0 0 1 0 1 0 1 0 0 0 0 0
    0 0 0 1 0 0 0 0 0 0 0 0 1 0
    0 0 1 0 0 0 0 0 0 0 0 0 1 0
    0 0 0 1 0 0 0 0 0 0 0 0 0 1
    0 0 1 0 0 0 0 0 0 0 0 0 0 0
    0 0 0 0 0 0 1 0 0 1 0 0 0 0
    1 0 0 0 0 0 0 0 0 0 0 1 0 0
    0 0 0 0 0 1 0 0 0 0 0 0 0 0
    0 0 0 0 0 0 0 0 0 1 0 0 0 1
    0 0 0 0 0 0 0 0 0 0 1 0 0 0
    0 0 0 0 0 0 1 0 0 0 0 0 0 0
 */