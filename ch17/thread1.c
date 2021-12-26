/*
 * thread1.c
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void *thread_function(void *arg);

int main( int argc, char **argv)
{
    int state;
    pthread_t t_id;
    void *t_return;

    state = pthread_create(&t_id, NULL, thread_function, NULL);
    if ( state != 0 )
    {
        puts("thread creation error !!!\n");
        exit(1);
    }
    printf("Created thread id : %ld\n", t_id);
    sleep(3);
    puts("main function ended \n");

    return 0;
}

void *thread_function(void *arg)
{
    int i;
    for ( i=0 ; i< 3; i++ )
    {
        sleep(2);
        printf("Executing Thread Function [%d]!!!\n", i);
    }
}