/*
 * mutex.c
 * compile : gcc -D_REENTRANT mutex.c -o mutex -lpthread
 * mutex is doorknob
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void *thread_increment(void *arg);
char thread1[] = "A Thread";
char thread2[] = "B Thread";

pthread_mutex_t mutx;

int sum = 0;

int main( int argc, char **argv)
{
    int state;
    pthread_t t1, t2;
    void *thread_result;

    state = pthread_mutex_init(&mutx, NULL);
    if (state)
    {
        puts("mutex initial error pthread_mutex_init()");
        exit(1);
    }
    pthread_create(&t1, NULL, thread_increment, &thread1);
    pthread_create(&t2, NULL, thread_increment, &thread2);

    pthread_join(t1, &thread_result);
    pthread_join(t2, &thread_result);

    printf("main function ended, result sum =%d \n", sum);

    pthread_mutex_destroy(&mutx);

    return 0;
}

void *thread_increment(void *arg)
{
    int i;

    for ( int i= 0 ; i < 5 ; i++ )
    {
        pthread_mutex_lock(&mutx);
        sleep(1);
        sum ++;
        printf("executed : %s, sum : %d \n", (char*)arg, sum);
        pthread_mutex_unlock(&mutx);
        sleep(1);
    }
}