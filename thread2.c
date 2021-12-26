/*
 * thread2.c
 * compile : gcc -D_REENTRANT thread2.c -o thread2 -lpthread
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

    //* Delay main process until the ends of thread
    state= pthread_join(t_id, &t_return);

    puts("main function ended \n");
    // heap memory free from being created in thread function
    free(t_return);

    return 0;
}

void *thread_function(void *arg)
{
    int i;
    char *p= (char *)malloc(20*sizeof(char));
    for ( i=0 ; i< 3; i++ )
    {
        sleep(2);
        printf("Executing Thread Function [%d]!!!\n", i);
    }

    return p;
}