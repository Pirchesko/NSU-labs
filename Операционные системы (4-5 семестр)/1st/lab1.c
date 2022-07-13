#include <stdio.h> //
#include <unistd.h> //-i, -s, -p, -c, для environ, для getopt()
#include <sys/types.h> //-i, -p
#include <ulimit.h> //-u, -U
#include <sys/resource.h> //-c
#include <sys/time.h> //-c
#include <stdlib.h> //-V

extern char** environ; //окружение пользователя (#include <unistd.h>)
//https://www.opennet.ru/man.shtml?topic=environ&category=5

int main(int argc, char* argv[])
{
    int input;
    char options[] = "ispuU:cC:dvV:"; //все нужные опции
    struct rlimit r_limit; //структура rlimit. Она используется системными вызовами getrlimit(2) и setrlimit(2)

    if (argc < 2)
    {
        printf("No arguments!\n");
        return 0;
    }

    while ((input = getopt(argc, argv, options)) != EOF) //https://www.opennet.ru/man.shtml?topic=getopt&russian=0&category=&submit=%F0%CF%CB%C1%DA%C1%D4%D8+man
    {
        switch (input)
        {
        case 'i': //!!!%ld??? Печатает реальные и эффективные идентификаторы пользователя и группы
            printf("Real id user (uid) = %ld\n", getuid()); //реальный (фактический) идентификатор пользователя
            printf("Effective id user (euid) = %ld\n", geteuid()); //эффективный идентификатор пользователя
            printf("Real id group (gid) = %ld\n", getgid()); //реальный (фактический) идентификатор группы
            printf("Effective id group (egid) = %ld\n", getegid()); //эффективный идентификатор группы
            break;//https://www.opennet.ru/man.shtml?topic=getuid&category=2&russian=0
        case 's': //Процесс становится лидером группы
            setpgrp(); //setpgid(0,0) одно и то же
            break; //https://www.opennet.ru/man.shtml?topic=setpgid&category=2&russian=0
        case 'p': //Печатает идентификаторы процесса, процесса-родителя и группы процессов
            printf("ID calling process = %ld\n", getpid()); //идентификатор ID текущего процесса
            printf("Parent ID calling process= %ld\n", getppid()); //идентификатор ID родительского процесса
            printf("Group ID  calling process = %ld\n", getpgid(0)); //идентификатор ID группы процессов
            break; //https://www.opennet.ru/man.shtml?topic=getpid&category=2&russian=0
        case 'u': //Печатает значение ulimit (ограничения длины файла)
            printf("ulimit = %ld\n", ulimit(UL_GETFSIZE)); //http://www.ccfit.nsu.ru/~deviv/courses/unix/unix/ng7ac7a.html (ulimit(UL_GETFSIZE, 0))
            break;//https://www.opennet.ru/man.shtml?topic=ulimit&category=3&russian=0
        case 'U':  //Изменяет значение ulimit
            if (ulimit(UL_SETFSIZE, atol(optarg)) == -1)
             {
                 printf("Error in size of ulimit\n");
             }
            break;
        case 'c': //Печатает размер в байтах core-файла, который может быть создан
            getrlimit(RLIMIT_CORE, &r_limit);
            printf("Core-file size = %ld\n", r_limit.rlim_cur);
            break; //http://www.ccfit.nsu.ru/~deviv/courses/unix/unix/ngfdf1.html
            //https://www.opennet.ru/man.shtml?topic=getrlimit&category=2&russian=0
        case 'C': //Изменяет размер core-файла
            getrlimit(RLIMIT_CORE, &r_limit);
            r_limit.rlim_cur = atol(optarg);
            setrlimit(RLIMIT_CORE, &r_limit);
        case 'd':
            printf("Working directory: %s\n", getcwd(NULL, 64));
            break;//https://www.opennet.ru/man.shtml?topic=getcwd&category=3&russian=0
        case 'v': //Распечатывает переменные среды и их значения
            printf("All environment variables:\n");
            char** now_environ;
            for (now_environ = environ; *now_environ; now_environ++)
            {
                printf("%s\n", *now_environ);
            }
            break;//http://www.ccfit.nsu.ru/~deviv/courses/unix/unix/ng12050.html
        case 'V': //Вносит новую переменную в среду или изменяет значение существующей переменной
            if (putenv(optarg) == -1)
            {
                printf("Error in name of environment variable\n");
            }
            break; //https://www.opennet.ru/man.shtml?topic=putenv&category=3&russian=0
        }
    }
    return 0;
}
