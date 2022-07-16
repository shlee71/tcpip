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
int sum1[]={1,1000};
int sum2[]={1001,2000};

int main( int argc, char **argv)
{
    pthread_t id_t1, id_t2;
    void *t_return;
    int state;

    pthread_create(&id_t1, NULL, thread_summation, (void*)sum1);
    pthread_create(&id_t2, NULL, thread_summation, (void*)sum2);
    sleep(2);
    printf("main function ended, result sum =%d \n", sum);
    // heap memory free from being created in thread function
        //* Delay main process until the ends of thread
    state= pthread_join(id_t1, &t_return);
        //* Delay main process until the ends of thread
    state= pthread_join(id_t2, &t_return);

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