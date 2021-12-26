/*
 * semaphore.c
 * compile : gcc -D_REENTRANT semaphore.c -o semaphore -lpthread
 * semaphore is integer number which cannot become minus.
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

void *thread_send(void *arg);
void *thread_recv(void *arg);

char thread1[] = "A Thread";
char thread2[] = "B Thread";
char thread3[] = "C Thread";

sem_t bin_sem;
int sum = 0;

int main( int argc, char **argv)
{
    int state;
    pthread_t t1, t2, t3;
    void *thread_result;

    state = sem_init(&bin_sem, 0, 0);
    if (state)
    {
        printf("sem_init() error\n");
        exit(1);
    }
    pthread_create(&t1, NULL, thread_send, &thread1);
    pthread_create(&t2, NULL, thread_recv, &thread2);
    pthread_create(&t3, NULL, thread_recv, &thread3);

    pthread_join(t1, &thread_result);
    pthread_join(t2, &thread_result);
    pthread_join(t3, &thread_result);

    printf("main function ended, result sum = %d \n", sum);

    sem_destroy(&bin_sem);

    return 0;
}

void *thread_send(void *arg)
{
    int i;

    for ( int i= 0 ; i < 4 ; i++ )
    {
        while( sum != 0)
            sleep(1);
        sum ++;
        printf("executed : %s, sum : %d \n", (char*)arg, sum);
        sem_post(&bin_sem);
        sleep(1);
    }
}

void *thread_recv(void *arg)
{
    int i;

    for ( int i= 0 ; i < 2; i++ )
    {
        sem_wait(&bin_sem);
        sum --;
        printf("executed : %s, sum : %d \n", (char*)arg, sum);
    }
}

