#pragma warning(disable : 4996);
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

//Syntax error
void error()
{
	printf("syntax error");
	exit(0);
}

//Разделение по пробелам и проверка на syntax error
void Working_with_input(char* text, char* primer)
{
	int i = 0;
	int j = 0;
	int numbers = 0;
	char previous_number = 0;
	int skobka_open = 0;
	int skobka_close = 0;
	int operations = 0;
	while (text[i] != '\n')
	{
		if (i >= 2)
		{
			previous_number = primer[j-2];
		}
		if ((text[i] >= '0') && (text[i] <= '9'))
		{
			if ((text[i + 1] >= '0') && (text[i + 1] <= '9'))
			{
				primer[j] = text[i];
				i++;
				j++;
			}
			else
			{
				primer[j] = text[i];
				primer[j + 1] = ' ';
				i++;
				j = j + 2;
				numbers++;
			}
		}
		else if ((text[i] == '(') || (text[i] == ')') || (text[i] == '+') || (text[i] == '-') || (text[i] == '*') || (text[i] == '/'))
		{
			primer[j] = text[i];
			primer[j + 1] = ' ';
			if (text[i] == '(')
			{
				skobka_open++;
			}
			if (text[i] == ')')
			{
				skobka_close++;
			}
			if ((text[i] == '+') || (text[i] == '-') || (text[i] == '*') || (text[i] == '/'))
			{
				operations++;
			}
			if (skobka_close > skobka_open)
			{
				error();
			}
			if ((skobka_close == skobka_open) && (previous_number != ')') && (skobka_close != 0) && (skobka_open != 0) && ((previous_number < '0') || (previous_number > '9')))
			{
				error();
			}
			i++;
			j = j + 2;
		}
		else if (text[i] == ' ')
		{
			i++;
		}
		else
		{
			error();
		}
	}
	if ((operations >= numbers) || (numbers >= operations + 2))
	{
		error();
	}
	if (skobka_open != skobka_close)
	{
		error();
	}
}

//Создание стека
typedef struct stack
{
	int data[1000];
	int size;
} stack;

//Длина стека
int d_stack(struct stack* stack)
{
	return stack->size;
}

//Push (добавление элемента в стек)
void push(stack* stack, int value)
{
	stack->size++;
	stack->data[stack->size] = value;
}

//Peek (берём последний элемент)
int peek(stack* stack)
{
	/*if (stack->size < 0)
	{
		exit(-101);
	}*/
	return stack->data[stack->size];
}

//Pop (берём последний элемент и удаляем его)
int pop(stack* stack)
{
	return stack->data[stack->size--];
}

// Приоритет операций
int Priority(char a)
{
	if ((a == '+') || (a == '-'))
	{
		return 1;
	}
	if ((a == '*') || (a == '/'))
	{
		return 2;
	}
	return 0;
}

//Выполнение операций в стеках
void do_calculation(stack* operations, stack* numbers, char next_operation)
{
	int output = 0;
	int number2 = pop(numbers);
	int number1 = pop(numbers);
	char operation = pop(operations);
	if (operation == '+')
	{
		output = number1 + number2;
	}
	else if (operation == '-')
	{
		output = number1 - number2;
	}
	else if (operation == '*')
	{
		output = number1 * number2;
	}
	else if (operation == '/')
	{
		if (number2 == 0)
		{
			printf("division by zero");
			exit(0);
		}
		output = number1 / number2;
	}
	else
	{
		error();
	}
	push(numbers, output);
	if (((d_stack(operations) >= 0) && (Priority(next_operation) <= Priority((char)peek(operations))) && (peek(operations) != '(')) || ((next_operation == ' ') && (d_stack(operations) >= 0)))
	{
		do_calculation(operations, numbers, next_operation);
	}
}

//Вытаскиваем цифру
int extract_number(char* primer, int i)
{
	int number = 0;
	while ((primer[i] != ' ') && (primer[i] != '\n'))
	{
		number *= 10;
		number += (int)primer[i] - (int)'0';
		i++;
	}
	return number;
}

//Смещаем i
int extract_i(char* primer, int i)
{
	while ((primer[i] != ' ') && (primer[i] != '\n'))
	{
		i++;
	}
	return i - 1;
}

//Сдвигаем к следующему символу
int sdvig_i(char* primer, int i)
{
	while ((primer[i] != ' ') && (primer[i] != '\n'))
	{
		i++;
	}
	return i + 1;
}

//Калькулятор
void Calculator(char* primer)
{
	stack operations;
	stack numbers;
	operations.size = -1;
	numbers.size = -1;
	int i = 0;
	while ((primer[i] != '\0') || (d_stack(&operations) >= 0))
	{
		if ((primer[i] >= '0') && (primer[i] <= '9'))
		{
			push(&numbers, extract_number(primer, i));
			i = extract_i(primer, i);
		}
		else
		{
			if (((Priority(primer[i]) > Priority((char)peek(&operations))) || (Priority((char)peek(&operations)) == 0) || (primer[i] == '(')) && (primer[i] != ' '))
			{
				if ((primer[i] == ')') && (peek(&operations) == '('))
				{
					pop(&operations);
				}
				else
				{
					push(&operations, (int)primer[i]);
				}
			}
			else
			{
				do_calculation(&operations, &numbers, primer[i]);
				if ((primer[i] != ' ') && (primer[i] != ')'))
				{
					push(&operations, (int)primer[i]);
				}
				if ((primer[i] == ')') && (peek(&operations) == '('))
				{
					pop(&operations);
				}
			}
		}
		if ((primer[i + 2] != '\0') || (d_stack(&numbers) == 0))
		{
			i = sdvig_i(primer, i);
		}
		else
		{
			i++;
		}
	}
	printf("%d", peek(&numbers));
}

int main()
{
	char text[2500] = { 0 };
	char primer[2500] = { 0 };
	if (fgets(text, 2499, stdin) == NULL)
	{
		return 0;
	}
	Working_with_input(text, primer);
	Calculator(primer);
	return 0;
}