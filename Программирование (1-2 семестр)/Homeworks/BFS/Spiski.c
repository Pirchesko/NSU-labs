/*
//Список - помещаем посещённые вершины
//Создание списка
typedef struct list
{
    int data; //номер элемента
    struct list* next; //указатель на следующий элемент
} list;
//Инициализация списка
list* init_list(int value) // а- значение первого узла
{
    list* checked;
    checked = (list*)malloc(sizeof(struct list));
    checked->data = value;
    checked->next = NULL;
    return(checked);
}
//Берём последнего
list* getLast_list(list* last)
{
    while (last->next)
    {
        last = last->next;
    }
    return last;
}
//Добавление в список
list* add_list(list* checked, int number)
{
    list* last = getLast_list(checked);
    list* temp = (list*)malloc(sizeof(list));
    checked->next = temp;
    temp->data = number;
    temp->next = NULL;
    return(temp);
}
*/