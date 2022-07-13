#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

char* inPath;
char* outPath;
//Файл ли это?
int isFile(struct stat buff)
{
    return S_ISREG(buff.st_mode);
}
//Каталог ли это?
int isDirectory(struct stat buff)
{
    return S_ISDIR(buff.st_mode);
}
//Присоединяем 2 путя
char* connectPaths(char* firstPath, char* secondPath)
{
    char* currentPath = (char*)malloc(strlen(firstPath) + strlen(secondPath) + 2);
    memcpy(currentPath, firstPath, strlen(firstPath));
    memcpy(currentPath + strlen(firstPath), "/", 1);
    memcpy(currentPath + strlen(firstPath) + 1, secondPath, strlen(secondPath) + 1);
    return currentPath;
}
//open() открыли файл
int openInFile(char* path)
{
    int status = -1;
    while (status < 0)
    {
        status = open(path, O_RDONLY);
        if ((status < 0) && (errno == EMFILE))
        {
            printf("Waiting opening file %s ...\n", path);
            sleep(1);
        }
        else if ((status < 0) && (errno == ENFILE))
        {
            printf("Waiting opening %s ...\n", outPath);
            sleep(1);
        }
        else
        {
            return status;
        }
    }
    return status;
}
//тупо сощдали файл со всеми его свойствами
int openOutFile(char* inPath, char* outPath)
{
    struct stat buff;
    if (stat(inPath, &buff) == -1)
    {
        printf("Error: Can't get information from path %s\n", inPath);
        return -1;
    }
    int status = -1;
    while (status < 0)
    {
        status = open(outPath, O_WRONLY | O_CREAT, buff.st_mode);
        if ((status < 0) && (errno == EMFILE))
        {
            printf("Waiting opening %s ...\n", outPath);
            sleep(1);
        }
        else if ((status < 0) && (errno == ENFILE))
        {
            printf("Waiting opening %s ...\n", outPath);
            sleep(1);
        }
        else
        {
            return status;
        }
    } 
    return status;
}
//mkdir() создание каталога
int makeOutPathDirectory(char* inPath, char* outPath)
{
    struct stat buff;
    if (stat(inPath, &buff) == -1)
    {
        printf("Error: Can't get information from path %s\n", inPath);
        return -2;
    }
    int makeDirStatus = mkdir(outPath, buff.st_mode);
    if ((makeDirStatus == -1) && (errno == EEXIST))
    {
        printf("Error: The named file exists.\n");
    }
    else if ((makeDirStatus == -1) && (errno == EACCES))
    {
        printf("Error: Access denided\n");
    } 
    else if ((makeDirStatus == -1) && (errno == ENOSPC))
    {
        printf("Error: No memory to create directory.\n");
    }
    return  makeDirStatus;
}
//opendir() открытие потока каталога
DIR* openInDirectory(char* path)
{
    DIR* openDirectory = NULL;
    while (openDirectory == NULL)
    {
        openDirectory = opendir(path);
        if ((openDirectory == NULL) && (errno == EACCES))
        {
            printf("Error: Access is denied %s\n", path);
            break;
        }
        else if ((openDirectory == NULL) && (errno == EMFILE)) //слишком много потоков
        {
            sleep(1);
        }
        else if ((openDirectory == NULL) && (errno == ENFILE)) //слишком много потоков
        {
            sleep(1);
        }
        else if ((openDirectory == NULL) && (errno == ENOMEM)) //не хватает памяти
        {
            sleep(1);
        }
        else if (openDirectory == NULL)
        {
            break;
        }
    }
    return openDirectory;
}
//Создание потока с нужной нам функцией
pthread_t newThreadCreate(void* (*function)(void*), void* param)
{
    pthread_t thread;
    int code = -1;
    while (code != 0)
    {
        code = pthread_create(&thread, NULL, function, param);
        if (code == EAGAIN)
        {
            printf("Waiting resources to create new thread...\n");
            sleep(1);
        }
    }
    pthread_detach(thread);
    return thread;
}
//Поток, который обрабатывает файлы
void* copyFile(void* param)
{
    char* getPath = (char*)param;
    char* allPathToGotInPath = connectPaths(inPath, getPath);
    char* allPathToGotOutPath = connectPaths(outPath, getPath);
    free(param);
    int inFile = openInFile(allPathToGotInPath);
    if (inFile == -1)
    {
        printf("Error: Can't open %s\n", allPathToGotInPath);

        free(allPathToGotInPath);
        free(allPathToGotOutPath);

        return (void*)EXIT_FAILURE;
    }
    int outFile = openOutFile(allPathToGotInPath, allPathToGotOutPath);
    if (outFile == -1)
    {
        printf("Error: Can't open %s\n", allPathToGotOutPath);

        if (close(inFile) == -1)
        {
            printf("Error: Can't close file %s\n", allPathToGotInPath);
        }
        free(allPathToGotInPath);
        free(allPathToGotOutPath);

        return (void*)EXIT_FAILURE;
    }
    int readStatus = 1;
    int writeStatus;
    char* buf = (char*)malloc(512);
    while (readStatus > 0)
    {
        while (1)
        {
            readStatus = read(inFile, buf, 512);
            if ((readStatus == -1) && (errno == EINTR))
            {
                continue;
            }
            else
            {
                break;
            }
        }
        if (readStatus == -1)
        {
            printf("Error: Read error %s\n", allPathToGotInPath);
            free(buf);
            if (close(inFile) == -1)
            {
                printf("Error: Can't close file %s\n", allPathToGotInPath);
            }
            if (close(outFile) == -1)
            {
                printf("Error: Can't close file %s\n", allPathToGotOutPath);
            }
            free(allPathToGotInPath);
            free(allPathToGotOutPath);
            pthread_exit(NULL);
        }
        while (1)
        {
            writeStatus = write(outFile, buf, 512);
            if ((writeStatus == -1) && (errno == EINTR))
            {
                continue;
            }
            else
            {
                break;
            }
        }
        if (writeStatus == -1)
        {
            printf("Error: Write error %s\n", allPathToGotOutPath);
            free(buf);
            if (close(inFile) == -1)
            {
                printf("Error: Can't close file %s\n", allPathToGotInPath);
            }
            if (close(outFile) == -1)
            {
                printf("Error: Can't close file %s\n", allPathToGotOutPath);
            }
            free(allPathToGotInPath);
            free(allPathToGotOutPath);
            pthread_exit(NULL);
        }
    }
    printf("Copied %s ...\n", allPathToGotInPath);
    free(allPathToGotInPath);
    free(allPathToGotOutPath);
    if (close(inFile) == -1)
    {
        printf("Error: Can't close file %s\n", allPathToGotInPath);
    }
    if (close(outFile) == -1)
    {
        printf("Error: Can't close file %s\n", allPathToGotOutPath);
    }
    free(buf);
    pthread_exit(NULL);
}
//Поток, который обрабатывает директории
void* copyDirectory(void* param)
{
    char* getPath = (char*)param;
    char* currentInPath = connectPaths(inPath, getPath);
    char* currentOutPath = connectPaths(outPath, getPath);
    char* spaceForDirent[sizeof(struct dirent) + pathconf(currentInPath, _PC_NAME_MAX) + 2];
    struct dirent* unknownFile = (struct dirent*)spaceForDirent;
    struct dirent* resultOfFile;
    struct stat buff;
    char* newPathWithGetDirectoryWithoutInPath;
    char* newPathWithGetDirectory;
    DIR* openDirectory = openInDirectory(currentInPath);
    if (openDirectory == NULL)
    {
        printf("Error: Can't open directory %s\n", currentInPath);
        free(currentInPath);
        free(currentOutPath);
        free(param);
        pthread_exit(NULL);
    }
    if (makeOutPathDirectory(currentInPath, currentOutPath) == -2)
    {
        if (closedir(openDirectory) == -1)
        {
            printf("Error: Can't close directory %s\n", currentInPath);
        }
        printf("Exist!\n");
        free(currentInPath);
        free(currentOutPath);
        free(param);
        pthread_exit(NULL);
    }
    while ((readdir_r(openDirectory, unknownFile, &resultOfFile) == 0) && (resultOfFile != NULL)) //чтение каталога
    {
        pthread_t thread;
        char* currentGetFromDirectoryPath = unknownFile->d_name;//берём имя записи файла
        if ((strcmp(currentGetFromDirectoryPath, ".") == 0) || (strcmp(currentGetFromDirectoryPath, "..") == 0))
        {
            continue;
        }
        newPathWithGetDirectory = connectPaths(currentInPath, currentGetFromDirectoryPath);
        if (stat(newPathWithGetDirectory, &buff) == -1)
        {
            printf("Error: Can't get information from path %s\n", newPathWithGetDirectory);
            continue;
        }
        free(newPathWithGetDirectory);
        newPathWithGetDirectoryWithoutInPath = connectPaths(getPath, currentGetFromDirectoryPath);
        if (isDirectory(buff))
        {
            thread = newThreadCreate(copyDirectory, newPathWithGetDirectoryWithoutInPath);
        }
        else if (isFile(buff))
        {
            thread = newThreadCreate(copyFile, newPathWithGetDirectoryWithoutInPath);
        }
    }
    printf("Copied %s ...\n", currentInPath);
    if (closedir(openDirectory) == -1)
    {
        printf("Error: Can't close directory %s\n", currentInPath);
    }
    free(currentInPath);
    free(currentOutPath);
    free(param);
    pthread_exit(NULL);
}

int main(int argc, char** argv)
{
    pthread_t thread;
    struct stat buff;
    if (argc < 3)
    {
        printf("Usage two arguments: path floder \"from\" and \"to\"\n", argv[0]);
        return 0;
    }
    inPath = argv[1];
    outPath = argv[2];
    if (stat(inPath, &buff) == -1)
    {
        printf("Error: Can't get information about path %s\n", inPath);
        return 0;
    }
    if (isDirectory(buff))
    {
        thread = newThreadCreate(copyDirectory, strdup(""));
    }
    else
    {
        printf("Error: Can't find directory to copy\n");
    }
    pthread_exit(NULL);
}

//https://qastack.ru/superuser/839719/why-is-r-recursive-necessary-when-copying-a-directory-in-linux //рекурсивное копирование директорий
//https://www.opennet.ru/man.shtml?topic=cp&category=1 //cp -R
//https://www.opennet.ru/man.shtml?topic=opendir&russian=0&category=&submit=%F0%CF%CB%C1%DA%C1%D4%D8+man //opendir()
//https://www.opennet.ru/man.shtml?topic=readdir&russian=0&category=&submit=%F0%CF%CB%C1%DA%C1%D4%D8+man //readdir()
//https://www.opennet.ru/man.shtml?topic=readdir_r&category=3&russian=5 //readdir_r()
//https://www.opennet.ru/man.shtml?topic=dirent.h&category=3&russian=5 //dirent.h
//https://ip-calculator.ru/blog/ask/chto-takoe-inody-v-linux-i-kak-oni-ispolzuyutsya/ //иноды
//https://losst.ru/simvolicheskie-i-zhestkie-ssylki-linux //жесткиеи мягкие ссылки
//https://firststeps.ru/linux/r.php?20 //Чтение директорий
//https://www.opennet.ru/man.shtml?topic=stat&category=2&russian=0 //stat()
//https://www.opennet.ru/man.shtml?topic=pathconf&category=3&russian=0 //pathconf()
//https://coderoad.ru/5256599/%D0%A7%D1%82%D0%BE-%D1%82%D0%B0%D0%BA%D0%BE%D0%B5-%D1%84%D0%B0%D0%B9%D0%BB%D0%BE%D0%B2%D1%8B%D0%B5-%D0%B4%D0%B5%D1%81%D0%BA%D1%80%D0%B8%D0%BF%D1%82%D0%BE%D1%80%D1%8B-%D0%BE%D0%B1%D1%8A%D1%8F%D1%81%D0%BD%D0%B5%D0%BD%D0%BD%D1%8B%D0%B5-%D0%BF%D1%80%D0%BE%D1%81%D1%82%D1%8B%D0%BC%D0%B8-%D1%81%D0%BB%D0%BE%D0%B2%D0%B0%D0%BC%D0%B8 //дискриптор
