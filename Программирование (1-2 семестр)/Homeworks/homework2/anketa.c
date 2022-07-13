#pragma warning(disable : 4996)
#include <stdio.h>
#include <malloc.h>

int hash(char* text, int k)
{
	int hash = 0;
	for (int i = 0; i < k; i++)
	{
		hash += (int)text[i] * i;
	}
	return hash;
}

//Получаем данные для создания массивов со студентами и преподами
void data_for_massives(int* students, int* max_prepod, int* max_symbols)
{
	char text; //Сюда временно вбиваются фамилии преподов
	int prepods = 0; //количтесво преводов у студентов
	int k = 0; //счётчик символов
	FILE* file = fopen("D:\\Files\\Visual Studio\\homework2\\Debug\\file.txt", "r");
	if (file == NULL)
	{
		printf("Ошибка считывания файла!\n");
		exit(0);
	}
	while (fscanf(file, "%c", &text) > 0)
	{
		if (text == ' ')
		{
			prepods++;
			k = -1;
		}
		if (text == '\n')
		{
			if (*max_prepod < prepods)
			{
				*max_prepod = prepods;
			}
			prepods = 0;
			(*students)++;
			k = -1;
		}
		if (k >= *max_symbols)
		{
			*max_symbols = k + 1;
		}
		k++;
	}
	fclose(file);
}

//Добавляем данные в массивы
void add_person(int** students_and_prepods_hash, char** name_student, char* text, int status, int k, int k_student, int k_prepod)
{
	if (status == 1)
	{
		for (int i = 1; i < k + 1; i++)
		{
			name_student[k_student][i] = text[i - 1];
		}
		name_student[k_student][0] = k;
		students_and_prepods_hash[k_student][0] = hash(text, k);
	}
	if ((status == 2) || (status == 3))
	{
		students_and_prepods_hash[k_student][k_prepod + 2] = hash(text, k);
		if (status == 3)
		{
			students_and_prepods_hash[k_student][1] = k_prepod + 1;
		}
	}
}

//Создаём массивы
void create_data(int** students_and_prepods_hash, char** name_student, int students, int max_prepod, int max_symbols)
{
	char text[30]; //временный массив фамилий
	int k = 0; //счётчик букв
	int k_student = 0; //счётчик студентов
	int k_prepod = -1; //счётчик преподов
	int status = 1; //1 - студент, 2 - препод, 3 - последний препод (нужно для фиксирования их количества)
	FILE* file = fopen("D:\\Files\\Visual Studio\\homework2\\Debug\\file.txt", "r");
	while (fscanf(file, "%c", &text[k]) > 0)
	{
		if (text[k] == ' ')
		{
			add_person(students_and_prepods_hash, name_student, text, status, k, k_student, k_prepod);
			k_prepod++;
			status = 2;
			k = -1;
		}
		else if (text[k] == '\n')
		{
			status = 3;
			add_person(students_and_prepods_hash, name_student, text, status, k, k_student, k_prepod);
			k_student++;
			k_prepod = -1;
			status = 1;
			k = -1;
		}
		k++;
	}
	fclose(file);
}

//Проверяем совпадения хешей перподавателей
int check_cross_prepods(int** students_and_prepods_hash, char** name_student, int student_1, int student_2)
{
	for (int i = 2; i < students_and_prepods_hash[student_1][1] + 1; i++)
	{
		for (int j = 2; j < students_and_prepods_hash[student_2][1] + 1; j++)
		{
			if (students_and_prepods_hash[student_1][i] == students_and_prepods_hash[student_2][j])
			{
				return 0; //у этой пары студентов имеется общий преподаватель
			}
		}
	}
	return 1;
}

//Выводим ответ в файл
void print_answer(FILE* file_out, char** name_student, int student_1, int student_2)
{
	for (int i = 1; i < name_student[student_1][0] + 1; i++)
	{
		fprintf(file_out, "%c", name_student[student_1][i]);
	}
	fprintf(file_out, " ");
	for (int i = 1; i < name_student[student_2][0] + 1; i++)
	{
		fprintf(file_out, "%c", name_student[student_2][i]);
	}
	fprintf(file_out, "\n");
}

//Прогоняем все пары студентов
void check_cross_students(int** students_and_prepods_hash, char** name_student, int students)
{
	FILE* file_out = fopen("D:\\Files\\Visual Studio\\homework2\\Debug\\file_out.txt", "w");
	for (int i = 0; i < students - 1; i++)
	{
		for (int j = i + 1; j < students; j++)
		{
			if (check_cross_prepods(students_and_prepods_hash, name_student, i, j) == 1)
			{
				print_answer(file_out, name_student, i, j);
			}
		}
	}
	fclose(file_out);
}

int main()
{
	//Навсякий случай русифицируем консольку
	system("chcp 1251");
	system("cls");

	//Перменные
	int max_prepod = 0; //максимальное количество преподов на одного студента
	int students = 1; //количество студентов
	int max_symbols = 0;//максимальное количество символов нужное на чью-либо фамилию

	data_for_massives(&students, &max_prepod, &max_symbols);

	//Массив хешов студентов и преподов. В 0-ом столбце хеш студента, в 1-ом столбце количество преподов у этого студента, а со 2-ого уже хэши преподов
	int** students_and_prepods_hash = (int**)malloc((students * (max_prepod + 2)) * sizeof(int*)); 
	for (int i = 0; i < students; i++)
	{
		students_and_prepods_hash[i] = (int*)malloc((max_prepod + 2) * sizeof(int*));;
	}
	//Массив имён студентов (понадобится для вывода). В 0-ом столбце хранится количество символов фамилии студента. Фамилии выводятся построчно.
	char** name_student = (char**)malloc((students * (max_symbols + 1)) * sizeof(char*)); 
	for (int i = 0; i < students; i++)
	{
		name_student[i] = (char*)malloc((max_symbols + 1) * sizeof(char*));
	}

	create_data(students_and_prepods_hash, name_student, students, max_prepod, max_symbols);

	check_cross_students(students_and_prepods_hash, name_student, students);

	return 0;
}

/*
Анкета содержит фамилию студента и список фамилий преподавателей, которые его обучают.
Одного студента обучают не более 10 преподавателей. Число студентов и число преподавателей не более 10^6.
Написать функцию, считывающую анкеты из одного файла и записывающую в другой файл все пары студентов, у которых списки преподавателей не пересекаются.
*/