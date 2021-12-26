/*
 * thread4.c
 * compile : gcc -D_REENTRANT thread4.c -o thread4 -lpthread
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void *thread_increment(void *arg);

int sum = 0;
#define num 10000

int main( int argc, char **argv)
{
    int i;
    pthread_t t_id[10];
    void *t_return;

    for ( i=0; i< 10; i++)
        pthread_create(&t_id[i], NULL, thread_increment, NULL);

    for ( i=0; i< 10; i++)
        pthread_join(t_id[i], &t_return);

    printf("main function ended, result sum =%d \n", sum);

    return 0;
}

void *thread_increment(void *arg)
{
    int i;

    for ( int i= 0 ; i <= num ; i++ )
    {
        sum += i;
    }
}