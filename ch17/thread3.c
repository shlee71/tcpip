/*
 * thread3.c
 * compile : gcc -D_REENTRANT thread3.c -o thread3 -lpthread
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void *thread_summation(void *arg);

int sum=0;
int sum1[]={1,5};
int sum2[]={6,10};

int main( int argc, char **argv)
{
    pthread_t id_t1, id_t2;
    void *t_return;

    pthread_create(&id_t1, NULL, thread_summation, (void*)sum1);
    pthread_create(&id_t2, NULL, thread_summation, (void*)sum2);
    sleep(2);
    printf("main function ended, result sum =%d \n", sum);
    // heap memory free from being created in thread function

    return 0;
}

void *thread_summation(void *arg)
{
    int start = ((int*)arg)[0];
    int end   = ((int*)arg)[1];

    printf("add from start [%d] to end [%d] \n", start, end);
    for ( int i= start ; i <= end ; i++ )
    {
        sum += i;
    }
}