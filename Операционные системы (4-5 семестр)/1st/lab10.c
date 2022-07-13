#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <wait.h>

int main(int argc, char **argv) {
    if (argc < 2)
    {
        perror("No arguments!");
        return 0;
    }
    int status;
    if (fork() == 0)
    {
        int error = execvp(argv[1], argv + 1); // int execv(const char *path, char *const argv[]); послдений аргумент должен заканчиваться на NULL (тут он автоматически так заканчивается)
        perror("Error in open file!");
        return error;
    }
    wait(&status);
    //printf("Exit status (WIFEXITED): %d\n", WIFEXITED(status));
    printf("Exit status (WEXITSTATUS): %d\n", WEXITSTATUS(status));
    //printf("Exit status (WIFSIGNALED): %d\n", WIFSIGNALED(status));
    //printf("Exit status (WTERMSIG): %d\n", WTERMSIG(status));
    //printf("Exit status (WIFSTOPPED): %d\n", WIFSTOPPED(status));
    //printf("Exit status (WSTOPSIG): %d\n", WSTOPSIG(status));
    //printf("Exit status (WIFCONTINUED): %d\n", WIFCONTINUED(status));
    //printf("Exit status (WCOREDUMP): %d\n", WCOREDUMP(status));
    exit(0);
    return 0;
}

//./lab10.exe /bin/cat text.txtttt