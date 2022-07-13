#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <signal.h>

sem_t detailA, detailB, detailC, widget;

int finish = 0;

void stop(int sign)
{
    if (sign == SIGINT)
    {
        finish = 1;
        signal(sign, SIG_IGN);
    }
}

void* createA (void* arg){
    while(1){
        if (finish == 1)
        {
            pthread_exit(NULL);
        }
        sleep(1);
        sem_post(&detailA);
        printf("A created\n");
    }
}

void* createB (void* arg){
    while(1){
        if (finish == 1)
        {
            pthread_exit(NULL);
        }
        sleep(2);
        sem_post(&detailB);
        printf("B created\n");
    }
}

void* createC (void* arg){
    while(1){
        if (finish == 1)
        {
            pthread_exit(NULL);
        }
        sleep(3);
        sem_post(&detailC);
        printf("C created\n");
    }
}

void* createwidget (void* arg){
    while(1){
        if (finish == 1)
        {
            pthread_exit(NULL);
        }
        sem_wait(&detailA);
        sem_wait(&detailB);
        sem_post(&widget);
        printf("AB created\n");
    }
}

void* createWidget (){
    while(1){
        if (finish == 1)
        {
            pthread_exit(NULL);
        }
        sem_wait(&widget);
        sem_wait(&detailC);
        printf("Widget created!\n");
    }
}

int main(){
    pthread_t aModule;
    pthread_t bModule;
    pthread_t cModule;
    pthread_t widgetModule;

    sem_init(&detailA, 0, 0);
    sem_init(&detailB, 0, 0);
    sem_init(&detailC, 0, 0);
    sem_init(&widget, 0, 0);

    signal(SIGINT, stop);

    pthread_create(&aModule, NULL, createA, NULL);
    pthread_create(&bModule, NULL, createB, NULL);
    pthread_create(&cModule, NULL, createC, NULL);
    pthread_create(&widgetModule, NULL, createwidget, NULL);
    createWidget();
}