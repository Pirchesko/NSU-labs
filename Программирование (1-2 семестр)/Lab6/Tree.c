#pragma warning(disable : 4996)
#include <stdio.h>
#include <malloc.h>

//Дерево
typedef struct tree
{
	int data;
	int height;
	struct tree* left;
	struct tree* right;
} tree;

//Высота дерева
int height(tree* tree)
{
	if (tree != NULL)
	{
		return tree->height;
	}
	else
	{
		return -1;
	}
}

//max
int max(int a, int b)
{
	if (a > b)
	{
		return a;
	}
	else
	{
		return b;
	}
}

//Значение баланса
tree* balance(tree* tree1, tree* tree2)
{
	int b = height(tree1) - height(tree2);
	return b;
}

//right_rotate 
tree* right_rotate(tree* tree)
{
	struct tree* left;
	//меняем местами ветки
	left = tree->left;
	tree->left = left->right;
	left->right = tree;
	//обновляем высоту
	tree->height = max(height(tree->left), height(tree->right)) + 1;
	left->height = max(height(left->left), tree->height) + 1;
	return left;
}

//left_rotate RR
tree* left_rotate(tree* tree)
{
	struct tree* right;
	//меняем местами ветки
	right = tree->right;
	tree->right = right->left;
	right->left = tree;
	//обновляем высоту
	tree->height = max(height(tree->left), height(tree->right)) + 1;
	right->height = max(height(right->right), tree->height) + 1;
	return right;
}

//leftright_rotate LR
tree* leftright_rotate(tree* tree)
{
	tree->left = left_rotate(tree->left);
	return right_rotate(tree);
}

//rightleft_rotate RL
tree* rightleft_rotate(tree* tree)
{
	tree->right = right_rotate(tree->right);
	return left_rotate(tree);
}

//Создание узла
tree* create(int data)
{
	tree* node = malloc(sizeof(*node));
	if (node != NULL)
	{
		node->data = data;
		node->left = NULL;
		node->right = NULL;
		node->height = 0;
	}
	return node;
}

//Добавление узла
tree* add(tree* tree, int data)
{
	if (tree == NULL) //если дерево пустое, создаём элемент
	{
		return create(data);
	}
	if (data <= tree->data) //добавление в левую часть
	{
		tree->left = add(tree->left, data);
		if (balance(tree->left, tree->right) == 2) //проверяем баланс
		{
			if (data <= (tree->left)->data) 
			{
				tree = right_rotate(tree); //LL - случай
			}
			else
			{
				tree = leftright_rotate(tree); //LR - случай
			}
		}
	}
	else if (data >= tree->data) //добавление в правую часть
	{
		tree->right = add(tree->right, data);
		if (balance(tree->right, tree->left) == 2) //проверяем баланс
		{
			if (data >= (tree->right)->data)
			{
				tree = left_rotate(tree); //RR - случай
			}
			else
			{
				tree = rightleft_rotate(tree); //RL - случай
			}
		}
	}
	tree->height = max(height(tree->left), height(tree->right)) + 1;
	return tree;
}

//Удаление дерева
void clear(tree* tree)
{
	if (tree == NULL)
	{
		return;
	}
	clear(tree->left);
	clear(tree->right);
	free(tree);
}

int main()
{
	//Перемннные
	int N;
	struct tree* tree = NULL;
	int v;
	int h;

	//Вводим данные
	if (scanf("%d", &N) != 1)
	{
		return 0;
	}
	for (int i = 0; i < N; i++)
	{
		if (scanf("%d", &v) != 1)
		{
			return 0;
		}
		tree = add(tree, v);
	}

	//Выводим высоту
	h = height(tree) + 1;
	printf("%d", h);

	//Чистим память
	clear(tree);
	return 0;
}