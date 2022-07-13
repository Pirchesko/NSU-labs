#pragma warning(disable : 4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct buffer_struct
{
    char bigbuf[9];
    int bb_end;
    int cnt;
}buffer_struct;

buffer_struct* initiatebuf_struct(buffer_struct* buf_struct)
{
    memset(buf_struct->bigbuf, '0', sizeof(buf_struct->bigbuf));
    buf_struct->cnt = 0; buf_struct->bb_end = 0;
    return buf_struct;
}

typedef struct node
{
    struct node* left;
    struct node* right;
    struct node* parent;
    bool active;
    char character;
    long count;
}node;

int Initiatetree_arrayay(node* tree_array, int freq_table[])
{
    int ends = 0;
    for (short i = 0; i < 256; i++)
    {
        if (freq_table[i] != 0)
        {
            tree_array[ends].count = freq_table[i];
            tree_array[ends].character = (char)i;
            tree_array[ends].left = NULL;
            tree_array[ends].right = NULL;
            tree_array[ends].parent = NULL;
            tree_array[ends].active = 1;
            ends++;
        }
    }
    return ends;
}

void FreeTree(node* tree)
{
    if (tree != NULL)
    {
        FreeTree(tree->right);
        FreeTree(tree->left);
        free(tree);
    }
}

void MakeTable(FILE* file_in, int freq_table[])
{
    memset(freq_table, 0, sizeof(int) * 256);
    unsigned char symbol = 0;
    while (!feof(file_in))
    {
        if (fscanf(file_in, "%c", &symbol))
            freq_table[symbol]++;
    }
}

void Encode(char character, char* buffer, node* tree_array)
{
    int i = 0;
    while (tree_array[i].character != character)
    {
        i++;
    }
    node* ptr = &tree_array[i];
    int currpos = 0;
    while (ptr->parent != NULL)
    {
        if (ptr == ptr->parent->left)
        {
            buffer[currpos] = '0';
            currpos++;
        }
        if (ptr == ptr->parent->right)
        {
            buffer[currpos] = '1';
            currpos++;
        }
        ptr = ptr->parent;
    }
    buffer[currpos] = '\0';
    for (int j = 0; j < currpos / 2; j++)
    {
        char tmp;
        tmp = buffer[j];
        buffer[j] = buffer[currpos - 1 - j];
        buffer[currpos - 1 - j] = tmp;
    }
}

int FindMinElement(node* tree_array, int end)
{
    int currmin = 0;
    while (tree_array[currmin].active == 0)
    {
        currmin++;
    }
    for (int i = 0; i < end; i++)
    {
        if ((tree_array[i].count < tree_array[currmin].count) && tree_array[i].active == 1)
            currmin = i;
    }
    tree_array[currmin].active = 0;
    return currmin;
}

node* BuildTree(int freq_table[], buffer_struct* buf_struct)
{
    node* tree_array = (struct node*)malloc(sizeof(node) * 2 * 256);
    buf_struct->cnt = Initiatetree_arrayay(tree_array, freq_table);
    int end = buf_struct->cnt;
    for (int i = 0; i < buf_struct->cnt - 1; i++)
    {
        int min1, min2;
        min1 = FindMinElement(tree_array, end);
        min2 = FindMinElement(tree_array, end);
        tree_array[min1].parent = &tree_array[end];
        tree_array[min2].parent = &tree_array[end];
        tree_array[end].left = &tree_array[min1];
        tree_array[end].right = &tree_array[min2];
        tree_array[end].parent = NULL;
        tree_array[end].count = tree_array[min1].count + tree_array[min2].count;
        tree_array[end].active = 1;
        end++;
    }
    return tree_array;
}

char zip(char buffer[])
{
    unsigned char x = 0;
    for (short i = 0; i < 8; i++)
    {
        if (buffer[7 - i] == '1')
        {
            x++;
        }
        if (i < 7)
        {
            x = x << 1;
        }
    }
    memset(buffer, '0', sizeof(char) * 9);
    return x;
}

void Submit(FILE* file_out, char* buffer, buffer_struct* buf_struct)
{
    for (short i = 0; i < (short)strlen(buffer); i++)
    {
        buf_struct->bigbuf[buf_struct->bb_end] = buffer[i];
        buf_struct->bb_end++;
        buf_struct->bigbuf[buf_struct->bb_end] = '\0';
        if (buf_struct->bb_end == 8)
        {
            char zipped_byte = zip(buf_struct->bigbuf);
            fputc(zipped_byte, file_out);
            buf_struct->bb_end = 0;
        }
    }
}

void Unzip(char symbol, char* buffer)
{
    unsigned char in = symbol;
    for (short i = 0; i < 8; i++)
    {
        unsigned char tmp = in % 2;
        if (tmp == 0) buffer[i] = '0';
        if (tmp == 1) buffer[i] = '1';
        in = in / 2;
    }
    buffer[8] = '\0';
}

char TakeOne(FILE* file_out, buffer_struct* buf_struct)
{
    char tmp;
    if (buf_struct->bb_end >= 8)
    {
        if (fread(&tmp, sizeof(char), 1, file_out))
            Unzip(tmp, buf_struct->bigbuf);
        buf_struct->bb_end = 0;
    }
    return buf_struct->bigbuf[buf_struct->bb_end++];
}

void Take(int bits_count, char* buf, FILE* file_out, buffer_struct* buf_struct)
{
    for (int i = 0; i < bits_count; i++)
    {
        buf[i] = TakeOne(file_out, buf_struct);
    }
    buf[bits_count] = '\0';
}

node* UnzipTree(FILE* file_out, buffer_struct* buf_struct)
{
    if (TakeOne(file_out, buf_struct) == '0')
    {
        node* node = (struct node*)calloc(sizeof(struct node), 1);
        node->left = UnzipTree(file_out, buf_struct);
        node->right = UnzipTree(file_out, buf_struct);
        return node;
    }
    else
    {
        char buf[9];
        Take(8, buf, file_out, buf_struct);
        node* leaf = (struct node*)calloc(sizeof(struct node), 1);
        leaf->character = zip(buf);
        leaf->left = NULL;
        leaf->right = NULL;
        return leaf;
    }
}

char Decode(FILE* file_out, node* tree, buffer_struct* buf_struct)
{
    while (tree->left != NULL)
    {
        if (TakeOne(file_out, buf_struct) != '0') tree = tree->right;
        else tree = tree->left;
    }
    return tree->character;
}

int Decompress(FILE* file_out, FILE* unfile_out, buffer_struct* buf_struct)
{
    if (file_out == NULL)
    {
        return 1;
    }
    int datasize;
    if (fread(&datasize, sizeof(int), 1, file_out))
    buf_struct->bb_end = 8;
    node* root = UnzipTree(file_out, buf_struct);
    for (int i = 0; i < datasize; i++)
    {
        char symbol = Decode(file_out, root, buf_struct);
        fputc(symbol, unfile_out);
    }
    FreeTree(root);
    return 0;
}

void zipTree(FILE* file_out, node* tree_array, node* n, buffer_struct* buf_struct)
{
    if (n->left != NULL)
    {
        Submit(file_out, "0", buf_struct);
        zipTree(file_out, tree_array, n->left, buf_struct);
    }
    else
    {
        char buffer[9];
        Submit(file_out, "1", buf_struct);
        Unzip(n->character, buffer);
        Submit(file_out, buffer, buf_struct);
    }
    if (n->right != NULL)
    {
        zipTree(file_out, tree_array, n->right, buf_struct);
    }
}

int Compress(FILE* file_in, FILE* file_out, int actual_start, buffer_struct* buf_struct)
{
    int freq_table[256];
    MakeTable(file_in, freq_table);
    node* tree_array = BuildTree(freq_table, buf_struct);
    char* buffer = (char*)calloc(buf_struct->cnt, 1);
    fseek(file_in, 0, SEEK_END);
    int datasize = ftell(file_in) - actual_start;
    fwrite(&datasize, sizeof(int), 1, file_out);
    fseek(file_in, actual_start, SEEK_SET);
    zipTree(file_out, tree_array, &tree_array[2 * buf_struct->cnt - 2], buf_struct);
    char character;
    while (fscanf(file_in, "%c", &character) > 0)
    {
        Encode(character, buffer, tree_array);
        Submit(file_out, buffer, buf_struct);
    }
    free(buffer);
    free(tree_array);
    if (buf_struct->bb_end != 0)
    {
        unsigned char drop_byte = zip(buf_struct->bigbuf);
        fwrite(&drop_byte, sizeof(char), 1, file_out);
    }
    return 0;
}

int main()
{
    FILE* file_in, * file_out;
    file_in = fopen("in.txt", "rb");
    if (file_in == NULL)
    {
        return 0;
    }
    file_out = fopen("out.txt", "wb");
    if (file_out == NULL)
    {
        fclose(file_in);
        return 0;
    }
    int count = 1;
    char direction, buf;
    if (fscanf(file_in, "%c", &direction))
    if (fscanf(file_in, "%c", &buf))
    while (buf != '\n')
    {
        count++;
        if (fscanf(file_in, "%c", &buf) == -1) break;
    }
    int actual_start = count + 1;
    fseek(file_in, actual_start, SEEK_SET);
    buffer_struct* buf_struct = (buffer_struct*)malloc(sizeof(buffer_struct));
    initiatebuf_struct(buf_struct);
    if (direction == 'c')
    {
        Compress(file_in, file_out, actual_start, buf_struct);
    }
    else if (direction == 'd')
    {
        Decompress(file_in, file_out, buf_struct);
    }
    free(buf_struct);
    fclose(file_in);
    fclose(file_out);
    return 0;
}
