#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h> //

#define PHILO 5
#define DELAY 5000
#define FOOD 50

pthread_mutex_t forks[PHILO];
pthread_t phils[PHILO];
sem_t semaphore; //
pthread_mutex_t foodlock;
int sleep_seconds = 0;

void get_fork(int phil, int fork, char* hand)
{
    pthread_mutex_lock(&forks[fork]);
    printf("Philosopher %d: got %s fork %d\n", phil, hand, fork);
}

void down_forks(int f1, int f2)
{
    sem_post(&semaphore); //
    pthread_mutex_unlock(&forks[f1]);
    pthread_mutex_unlock(&forks[f2]);
}

int food_on_table()
{
    static int food = FOOD;
    int myfood;
    pthread_mutex_lock(&foodlock);
    if (food > 0)
    {
        food--;
    }
    myfood = food;
    pthread_mutex_unlock(&foodlock);
    return myfood;
}

void* philosopher(void* num)
{
    int id, cnt = 0;
    int left_fork, right_fork, f;
    id = (int)num;
    printf("Philosopher %d sitting down to dinner.\n", id);
    right_fork = id;
    left_fork = id + 1;
    if (left_fork == PHILO)
    {
        left_fork = 0;
    }
    while (f = food_on_table())
    {
        cnt++;
        if (id == 1)
        {
            sleep(sleep_seconds);
        }
        printf("Philosopher %d: get dish %d.\n", id, f);
        sem_wait(&semaphore); //
        get_fork(id, right_fork, "right");
        get_fork(id, left_fork, "left ");
        printf("Philosopher %d: eating.\n", id);
        usleep(DELAY * (FOOD - f + 1));
        down_forks(left_fork, right_fork);
    }
    printf("Philosopher %d is done eating, ate %d.\n", id, cnt);
    return (NULL);
}

int main(int argn, char **argv) 
{
    int i;
    if (argn == 2)
    {
        sleep_seconds = atoi(argv[1]);
    }
    pthread_mutex_init(&foodlock, NULL);
    sem_init(&semaphore, 0, PHILO - 1); //
    for (i = 0; i < PHILO; i++) 
    {
        pthread_mutex_init(&forks[i], NULL);
    }
    for (i = 0; i < PHILO; i++)
    {
        pthread_create(&phils[i], NULL, philosopher, (void *) i);
    }
    for (i = 0; i < PHILO; i++)
    {
        pthread_join(phils[i], NULL);
    }
    sem_destroy(&semaphore); //
    return 0;
}