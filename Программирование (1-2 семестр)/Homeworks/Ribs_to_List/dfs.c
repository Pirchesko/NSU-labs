//#include <stdio.h>
//#include <stdlib.h>
//
//typedef struct node 
//{
//	int index;
//	struct node* next;
//} node;
//
//void dfs(int v, int used[], node** graph)
//{
//	used[v] = 1;
//	for (node* it = graph[v]; it; it = it->next)
//	{
//		if (!used[it->index])
//		{
//			dfs(it->index, used, graph);
//		}
//	}
//}
//
//void AddNode(int index, node** head)
//{
//	if (!head)
//	{
//		return;
//	}
//	node* a = (node*)malloc(sizeof(node));
//	a->index = index;
//	a->next = *head;
//	*head = a;
//}
//
//int main() {
//	int node_count;
//	int edge_count;
//	scanf("%d %d", &node_count, &edge_count);
//	node** graph = (node**)malloc(node_count * sizeof(node*));
//	for (int i = 0; i < node_count; ++i)
//	{
//		graph[i] = NULL;
//	}
//
//	for (int i = 0; i < edge_count; ++i)
//	{
//		int from, to;
//		scanf("%d %d", &from, &to);
//		AddNode(to, &graph[from]);
//		AddNode(from, &graph[to]);
//	}
//
//	int comp_count = 0;
//	int* used = (int*)malloc(node_count * sizeof(int));
//	for (int i = 0; i < node_count; ++i)
//	{
//		used[i] = 0;
//	}
//
//	for (int i = 0; i < node_count; ++i)
//	{
//		if (!used[i])
//		{
//			dfs(i, used, graph);
//			++comp_count;
//		}
//	}
//	printf("%d", comp_count);
//
//	for (int i = 0; i < node_count; ++i)
//	{
//		node* it = graph[i];
//		while (it) 
//		{
//			node* next = it->next;
//			free(it);
//			it = next;
//		}
//	}
//	free(graph);
//
//	free(used);
//	return 0;
//}