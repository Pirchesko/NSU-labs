#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
    FILE* ptrs[2];
    char num[4];

    srand(time(NULL));
    p2open("/bin/sort", ptrs);

    for (int i = 1; i <= 100; i++)
    {
        sprintf(num, "%02d\n", rand() % 100);
        fputs(num, ptrs[0]);
    }
    fclose(ptrs[0]);

    int  k = 1;
    while (fgets(num, 4, ptrs[1]) != NULL)
    {
        num[2] = '\0';
        printf("  %s", num);
        if ((i++ % 10) == 0)
        {
            putchar('\n');
        }
    }
    putchar('\n');
}

//-lgen