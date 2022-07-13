#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>

#define PHILO 5
#define DELAY 30000
#define FOOD 30

pthread_mutex_t forks[PHILO];
pthread_mutex_t allForks;
pthread_t phils[PHILO];
pthread_cond_t cond;
pthread_mutex_t foodlock;

void get_forks(int phil, int left_fork, int right_fork)
{
    int res;
    pthread_mutex_lock(&allForks);
    do
    {
        if ((res = pthread_mutex_trylock(&forks[left_fork])) == 0)
        {
            res = pthread_mutex_trylock(&forks[right_fork]);
            if (res != 0)
            {
                pthread_mutex_unlock(&forks[left_fork]);
            }
        }
        if (res != 0)
        {
            pthread_cond_wait(&cond, &allForks);
        }
    } while (res != 0);
    pthread_mutex_unlock(&allForks);
}

void down_forks(int f1, int f2)
{
    pthread_mutex_lock(&allForks);
    pthread_mutex_unlock(&forks[f1]);
    pthread_mutex_unlock(&forks[f2]);
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&allForks);
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
    int id = *((int*)num);
    int left_fork, right_fork, f;
    int cnt = 0;
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
        printf("Philosopher %d: get dish %d.\n", id, f);
        get_forks(id, left_fork, right_fork);
        printf("Philosopher %d: eating.\n", id);
        usleep(DELAY * (FOOD - f + 1));
        down_forks(left_fork, right_fork);
    }
    printf("Philosopher %d is done eating\n", id);
    pthread_exit(NULL);
}

int main(int argc, char* argv[])
{
    int i, j, error_num;
    pthread_mutex_init(&foodlock, NULL);
    pthread_mutex_init(&allForks, NULL);
    pthread_cond_init(&cond, NULL);

    for (i = 0; i < PHILO; i++)
    {
        pthread_mutex_init(&forks[i], NULL);
    }
    for (i = 0; i < PHILO; i++)
    {
        pthread_create(&phils[i], NULL, philosopher, &i);
    }
    for (i = 0; i < PHILO; i++)
    {
        pthread_join(phils[i], NULL);
    }

    pthread_mutex_destroy(&allForks);
    pthread_mutex_destroy(&foodlock);
    pthread_cond_destroy(&cond);
    for (i = 0; i < PHILO; i++)
    {
        pthread_mutex_destroy(&forks[i]);
    }
    pthread_exit(NULL);
}