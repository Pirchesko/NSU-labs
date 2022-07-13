#include <stdio.h>

int main()
{
    FILE* fptr;
    char* line = { "ThIsIsThEoRiGiNaLtExT\n" };
    fptr = popen("./lab26_go.exe", "w");
    fputs(line, fptr);
    pclose(fptr);
    return 0;
}
//http://www.ccfit.nsu.ru/~deviv/courses/unix/unix/ngaf0a9.html