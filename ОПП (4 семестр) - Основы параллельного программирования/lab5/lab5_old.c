#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include <unistd.h>
#include <stdbool.h>

#define L 5000
#define NUM_LISTS 3
#define NUM_TASKS 100

pthread_mutex_t mutex;
int tasks[NUM_TASKS];
pthread_mutex_t mutexForRemainingTasks;
int numRemainingTasks;
int numCompletedTasks;

int size;
int rank;

double globalRes = 0;

void generateTasks(int* tasksArray, int numTasks, int iterCounter)
{
    for (int i = 0; i < numTasks; i++)
    {
        tasksArray[i] = abs(50 - i % 100) * abs(rank - (iterCounter % size)) * L;
    }
}

void do_work()
{
    pthread_mutex_lock(&mutexForRemainingTasks);
    int currentRemainingTasks = numRemainingTasks;
    pthread_mutex_unlock(&mutexForRemainingTasks);

    for (int j = 0; j < currentRemainingTasks; j++)
    {
        pthread_mutex_lock(&mutex);
        int repeatNum = tasks[j];
        pthread_mutex_unlock(&mutex);

        for (int m = 0; m < repeatNum; m++)
        {
            globalRes += sin(m);
        }
        numCompletedTasks++;
        pthread_mutex_lock(&mutexForRemainingTasks);
        currentRemainingTasks = numRemainingTasks;
        pthread_mutex_unlock(&mutexForRemainingTasks);
    }
    pthread_mutex_lock(&mutexForRemainingTasks);
    numRemainingTasks = 0;
    pthread_mutex_unlock(&mutexForRemainingTasks);
}

void* executeTasks(void* args)
{
    for (int i = 0; i < NUM_LISTS; ++i)
    {
        double startIter = MPI_Wtime();
        generateTasks(tasks, NUM_TASKS, i);
        numRemainingTasks = NUM_TASKS;
        numCompletedTasks = 0;
        do_work();
        for (int k = 0; k < size; ++k)
        {
            if (k != rank)
            {
                int numNewTasks;
                MPI_Send(&rank, 1, MPI_INT, k, 0, MPI_COMM_WORLD);
                MPI_Recv(&numNewTasks, 1, MPI_INT, k, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                if (numNewTasks > 0)
                { 
                    MPI_Recv(tasks, numNewTasks, MPI_INT, k, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    numRemainingTasks = numNewTasks;
                    do_work(); 
                }
            }
        }
        double iterationTime = MPI_Wtime() - startIter; 
        double minIterationTime, maxIterationTime;
        MPI_Allreduce(&iterationTime, &minIterationTime, 1, MPI_DOUBLE, MPI_MIN, MPI_COMM_WORLD);
        MPI_Allreduce(&iterationTime, &maxIterationTime, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
     /*   for (int j = 0; j < size; ++j) {
            MPI_Barrier(MPI_COMM_WORLD);
            if (rank == 0 && j == 0) {
                printf("iter#%d\n", i);
                printf("Imbalance time = %f\n", maxIterationTime - minIterationTime);
                printf("Imbalance in %% = %f\n", ((maxIterationTime - minIterationTime) / maxIterationTime) * 100);
            }
            if (rank == j) {
                printf("PROC%d: Completed tasks:\t\t%d\n", rank, numCompletedTasks);
                printf("PROC%d: Global result\t\t%f\n", rank, globalRes);
                printf("PROC%d: IterationTime\t\t%f\n", rank, iterationTime);
                printf("\n");
            }
        }*/
        //MPI_Barrier(MPI_COMM_WORLD);
    }
    int stopJob = -1;
    MPI_Send(&stopJob, 1, MPI_INT, rank, 0, MPI_COMM_WORLD);

    return NULL;
}

void* waitForTasks(void* args)
{
    while (true)
    {
        int requestRank;
        MPI_Status status;
        MPI_Recv(&requestRank, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
        if (requestRank == -1)
        {
            pthread_exit(NULL);
        }
        pthread_mutex_lock(&mutex);
        if (numRemainingTasks > 10)
        {
            int numShareTasks = numRemainingTasks / 2;
            pthread_mutex_lock(&mutexForRemainingTasks);
            numRemainingTasks -= numShareTasks;
            pthread_mutex_unlock(&mutexForRemainingTasks);

            MPI_Send(&numShareTasks, 1, MPI_INT, status.MPI_SOURCE, 1, MPI_COMM_WORLD);
            MPI_Send(&tasks[numShareTasks - 1], numShareTasks, MPI_INT, status.MPI_SOURCE, 1, MPI_COMM_WORLD);
        }
        else
        {
            int numShareTasks = 0;
            MPI_Send(&numShareTasks, 1, MPI_INT, status.MPI_SOURCE, 1, MPI_COMM_WORLD);
        }
        pthread_mutex_unlock(&mutex);
    }
}

int createThreads(pthread_t* waiter, pthread_t* executor)
{
    pthread_attr_t attrsForExecutor;
    if (pthread_attr_init(&attrsForExecutor) != 0)
    {
        perror("Cannot initialize attributes for executor");
        return -1;
    }
    pthread_attr_t attrsForWaiter;
    if (pthread_attr_init(&attrsForWaiter) != 0)
    {
        pthread_attr_destroy(&attrsForExecutor);
        perror("Cannot initialize attributes for waiter");
        return -1;
    }
    pthread_attr_setdetachstate(&attrsForWaiter, PTHREAD_CREATE_JOINABLE);
    pthread_attr_setdetachstate(&attrsForExecutor, PTHREAD_CREATE_JOINABLE);
    if (pthread_create(waiter, &attrsForWaiter, executeTasks, NULL) == -1)
    {
        perror("Could not create waiter thread");
        pthread_attr_destroy(&attrsForWaiter);
        return -1;
    }
    pthread_attr_destroy(&attrsForWaiter);
    if (pthread_create(executor, &attrsForExecutor, waitForTasks, NULL) == -1)
    {
        perror("Could not create executor thread");
        pthread_attr_destroy(&attrsForExecutor);
        return -1;
    }
    pthread_attr_destroy(&attrsForExecutor);
    if (pthread_join(*waiter, NULL))
    {
        perror("Could not join waiter thread");
        return -1;
    }
    if (pthread_join(*executor, NULL))
    {
        perror("Could not join execute thread");
        return -1;
    }
    return 0;
}

int main(int argc, char** argv)
{
    int providedLevel;
    MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &providedLevel);
    if (providedLevel != MPI_THREAD_MULTIPLE)
    {
        printf("This MPI realization is not thread-compliant.\n");
        MPI_Finalize();
        return -1;
    }

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    double startTime = MPI_Wtime();

    pthread_t waiter;
    pthread_t executor;
    if (createThreads(&waiter, &executor) != 0)
    {
        printf("Can not create threads.\n");
        MPI_Finalize();
        return -1;
    }

    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&mutexForRemainingTasks, NULL);
    double time = MPI_Wtime() - startTime;
    if (rank == 0)
    {
        printf("Global time in work:\t\t%f\n", time);
    }
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&mutexForRemainingTasks);

    MPI_Finalize();
    return 0;
}
