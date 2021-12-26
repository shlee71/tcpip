/* 
 * sigint.c
 */

#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void handler(int sig);
int main(int argc, char **argv)
{
    int state;
    int num = 0;

    signal(SIGINT, handler);
    while(1)
    {
        printf("%d : Waiting for singnal \n", num++);
        sleep(2);

        if ( num > 5)
            break;
    }

    return 0;
}

void handler(int sig)
{
    signal(SIGINT, handler);
    printf("received signal %d \n", sig);
}
