#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <mpi.h>
#include <pthread.h>

#define REQUEST_TAG 10
#define ANSWER_TAG 20
#define SUCCESS 500
#define FAIL 404
#define NEED_TASKS 220
#define TURN_OFF 312
#define TASKS_IN_LIST 800
#define L 10000

typedef struct Task
{
    int repeatNum;
} Task;

typedef Task TaskList[TASKS_IN_LIST];
int rank, size;
int iterCounter = 0;
int iterMax = 5;
int curTask = 0;
int listSize = 0;
double globalRes = 0;
TaskList taskList;
pthread_mutex_t list_mutex;
MPI_Datatype MPI_TASK;

void getTaskList(int iter)
{
    listSize = TASKS_IN_LIST;
    for (int i = rank * TASKS_IN_LIST; i < (rank + 1) * TASKS_IN_LIST; i++)
    {
        taskList[i % TASKS_IN_LIST].repeatNum = abs(TASKS_IN_LIST / 2 - i % TASKS_IN_LIST) * abs(rank - (iterCounter % size)) * L;
    }
}

int getTaskFrom(int from)
{
    int flag = NEED_TASKS;
    MPI_Send(&flag, 1, MPI_INT, from, REQUEST_TAG, MPI_COMM_WORLD);
    MPI_Recv(&flag, 1, MPI_INT, from, ANSWER_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    if (flag == FAIL)
    {
        return FAIL;
    }
    Task recvTask;
    MPI_Recv(&recvTask, 1, MPI_TASK, from, ANSWER_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    pthread_mutex_lock(&list_mutex);
    taskList[listSize] = recvTask;
    listSize++;
    pthread_mutex_unlock(&list_mutex);
    return SUCCESS;
}

void doWork(Task task)
{
    for (int i = 0; i < task.repeatNum; i++)
    {
        globalRes += sin(i);
    }
}

void* SenderThread(void* args)
{
    int flag;
    while (iterCounter < iterMax)
    {
        MPI_Status status;
        MPI_Recv(&flag, 1, MPI_INT, MPI_ANY_SOURCE, REQUEST_TAG, MPI_COMM_WORLD, &status);
        if (flag == TURN_OFF)
        {
            break;
        }
        pthread_mutex_lock(&list_mutex);
        if (curTask >= listSize - 1)
        {
            pthread_mutex_unlock(&list_mutex);
            flag = FAIL;
            MPI_Send(&flag, 1, MPI_INT, status.MPI_SOURCE, ANSWER_TAG, MPI_COMM_WORLD);
            continue;
        }
        listSize--;
        Task sendTask = taskList[listSize];
        pthread_mutex_unlock(&list_mutex);
        flag = SUCCESS;
        MPI_Send(&flag, 1, MPI_INT, status.MPI_SOURCE, ANSWER_TAG, MPI_COMM_WORLD);
        MPI_Send(&sendTask, 1, MPI_TASK, status.MPI_SOURCE, ANSWER_TAG, MPI_COMM_WORLD);
    }
    return NULL;
}

void* ExecuterThread(void* args)
{
    MPI_Barrier(MPI_COMM_WORLD);
    struct timespec start, end;
    iterCounter = 0;
    while (iterCounter < iterMax)
    {
        int tasksDone = 0;
        int hasTasks = 1;
        pthread_mutex_lock(&list_mutex);
        curTask = 0;
        getTaskList(iterCounter);
        pthread_mutex_unlock(&list_mutex);
        clock_gettime(CLOCK_MONOTONIC, &start);
        while (hasTasks)
        {
            pthread_mutex_lock(&list_mutex);
            if (curTask < listSize)
            {
                Task task = taskList[curTask];
                pthread_mutex_unlock(&list_mutex);
                doWork(task);
                tasksDone++;
                pthread_mutex_lock(&list_mutex);
                curTask++;
                pthread_mutex_unlock(&list_mutex);
                continue;
            }
            curTask = 0;
            listSize = 0;
            pthread_mutex_unlock(&list_mutex);
            hasTasks = 0;
            for (int i = 0; i < size; i++)
            {
                if (i == rank)
                {
                    continue;
                }
                if (getTaskFrom(i) == SUCCESS)
                {
                    hasTasks = 1;
                }
            }
        }
        clock_gettime(CLOCK_MONOTONIC, &end);
        double timeTaken = end.tv_sec - start.tv_sec + 0.000000001 * (end.tv_nsec - start.tv_nsec);
        printf("%d; %d; %d; %lf; %lf; ; \n", iterCounter, rank, tasksDone, timeTaken, globalRes);
        double minTime, maxTime;
        MPI_Reduce(&timeTaken, &minTime, 1, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD);
        MPI_Reduce(&timeTaken, &maxTime, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
        if (rank == 0)
        {
            printf("%d; ; ; ; ; %lf; %lf%\n", iterCounter, maxTime - minTime, (maxTime - minTime) / maxTime * 100);
        }
        MPI_Barrier(MPI_COMM_WORLD);
        iterCounter++;
    }
    int flag = TURN_OFF;
    MPI_Send(&flag, 1, MPI_INT, rank, REQUEST_TAG, MPI_COMM_WORLD);
    return NULL;
}

void createTaskType()
{
    const int num = 1;
    int lenOfBlocks[num];
    lenOfBlocks[0] = 1;
    MPI_Datatype types[num];
    types[0] = MPI_INT;
    MPI_Aint offsets[num];
    offsets[0] = offsetof(Task, repeatNum);
    MPI_Type_create_struct(num, lenOfBlocks, offsets, types, &MPI_TASK);
    MPI_Type_commit(&MPI_TASK);
}

int createThreads(pthread_t* sender, pthread_t* executor)
{
    pthread_attr_t attrsForExecutor;
    if (pthread_attr_init(&attrsForExecutor) != 0)
    {
        printf("Cannot initialize attributes for executor");
        return FAIL;
    }
    pthread_attr_t attrsForSender;
    if (pthread_attr_init(&attrsForSender) != 0)
    {
        pthread_attr_destroy(&attrsForExecutor);
        printf("Cannot initialize attributes for waiter");
        return FAIL;
    }
    pthread_attr_setdetachstate(&attrsForSender, PTHREAD_CREATE_JOINABLE);
    pthread_attr_setdetachstate(&attrsForExecutor, PTHREAD_CREATE_JOINABLE);
    if (pthread_create(sender, &attrsForSender, SenderThread, NULL) != 0)
    {
        printf("Could not create waiter thread");
        pthread_attr_destroy(&attrsForSender);
        return FAIL;
    }
    pthread_attr_destroy(&attrsForSender);
    if (pthread_create(executor, &attrsForExecutor, ExecuterThread, NULL) != 0)
    {
        printf("Could not create executor thread");
        pthread_attr_destroy(&attrsForExecutor);
        return FAIL;
    }
    pthread_attr_destroy(&attrsForExecutor);
    if (pthread_join(*sender, NULL))
    {
        printf("Could not join waiter thread");
        return FAIL;
    }
    if (pthread_join(*executor, NULL))
    {
        printf("Could not join execute thread");
        return FAIL;
    }
    return SUCCESS;
}

int main(int argc, char** argv)
{
    int provided;
    MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);
    if (provided != MPI_THREAD_MULTIPLE)
    {
        printf("This MPI realization is not thread-compliant.\n");
        MPI_Finalize();
        return 0;
    }
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    pthread_mutex_init(&list_mutex, NULL);
    createTaskType();
    printf("Iterarion; Proccess; Tasks done; Time taken; Global result; Imbalance time; %\n");
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    pthread_t sender;
    pthread_t executor;
    if (createThreads(&sender, &executor) != SUCCESS)
    {
        printf("Can not create threads.\n");
        MPI_Finalize();
        return 0;
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    double timeTaken = end.tv_sec - start.tv_sec + 0.000000001 * (end.tv_nsec - start.tv_nsec);
    double minTime, maxTime;
    MPI_Reduce(&timeTaken, &minTime, 1, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD);
    MPI_Reduce(&timeTaken, &maxTime, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    if (rank == 0)
    {
        printf("Total time: min=%lf, max=%lf\n", minTime, maxTime);
    }
    pthread_mutex_destroy(&list_mutex);
    MPI_Type_free(&MPI_TASK);
    MPI_Finalize();
    return 0;
}