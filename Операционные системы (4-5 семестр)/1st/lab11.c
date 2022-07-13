#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <wait.h>
#include <time.h>

int execvpe(const char *file, char *const argv[], char * myenv[])
{
    extern char **environ;
    environ = myenv;
    char** now_environ;
    for (now_environ = environ; *now_environ; now_environ++)
    {
        printf("%s\n", *now_environ);
    }
    return execvp(file, argv);
}

int main(int argc, char *argv[], char* myenv[])
{
    if (argc < 2)
    {
        perror("No arguments!");
        return 0;
    }
    /*static char *myenv[] =
    { 
        "TZ=America/Los_Angeles",
        (char *) 0 
    };*/
    if (putenv("TZ=America/Los_Angeles") != 0)
    {
        perror("Bad env!");
    }
    if (execvpe(argv[1], argv + 1, myenv) == -1)
    {
        perror("Error in open file!");
    }
    return 0;
}

//./lab11.exe env /bin/cat text.txt