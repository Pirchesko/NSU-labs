#include <stdio.h>

int main()
{
    FILE* stat = fopen("statistic.txt", "a");
    fprintf(stat, "\n");
    fclose(stat);
    return 0;
}