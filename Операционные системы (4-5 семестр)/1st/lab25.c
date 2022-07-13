#include <stdio.h> //perror()
//#include <sys/types.h>
#include <unistd.h> //read(), close(), write()
#include <ctype.h> //toupper(), islower()
#include <string.h>

int main(int argc, char** argv)
{
    int pid;
    int fd[2];
    static char* lines = { "ThIsIsOrIgInAlTeXt\n" };
    char input[1000];
    int rtn;
    if (pipe(fd) == -1)
    {
        perror(argv[0]); //не удалось создать программный канал
        return 0;
    }
    pid = fork();
    /*printf("fd[0] = %d\n", fd[0]);
    printf("fd[1] = %d\n", fd[1]);*/
    if (pid > 0)
    {
        close(fd[0]);
        write(fd[1], lines, strlen(lines));
        write(0, lines, strlen(lines));
        close(fd[1]);
    }
    else if (pid == 0)
    {
        close(fd[1]);
        while ((rtn = read(fd[0], input, 1000)) > 0) //возращает количество байтов, которые были считаны
        {
            for (int i = 0; i < rtn; i++)
            {
                if (islower(input[i])) //если символ нижнего регистра
                {
                    input[i] = toupper(input[i]); //преобразуем символ в заглавный фрпмат
                }
            }
            write(0, input, rtn);
        }
        close(fd[0]);
    }
    else
    {
        perror(argv[0]);
        return 0;
    }
    return 0;
}
//http://ccfit.nsu.ru/~deviv/courses/unix/unix/ngac3cf.html