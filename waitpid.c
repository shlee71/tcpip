/* 
 * waitpid.c
 * zombie process »Æ¿Œ
 * 1. ps -u | grep  waitpid
 * 2. ps -ef | grep waitpid
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main( int argc, char **argv)
{
    pid_t pid,child;
    int data = 10;
    int state;

    pid=fork();

    if (pid == -1)
    {
        printf("fork() failed, process id : %d \n", pid);
        return 1;
    }

    if( pid > 0 )
    {
        pid=fork();

        if (pid == -1)
        {
            printf("fork() failed, process id : %d \n", pid);
            return 1;
        }
    }

    printf("fork() success, proess id : [%d] \n", pid);

    if( pid == 0 ) // child process
    {
        
        data += 10;
        sleep(10);
    }
    else          // parent process
    {
        data -= 10;
        int count_down = 9;
        printf("Before waitpid\n");
        system("ps -ef | grep waitpid");
        do {
            sleep(1);
            printf("Count down [%d]\n",count_down--);
            child=waitpid(-1, &state, WNOHANG);
            if (child > 0)
            {
                printf("child process id  [%d] \n", child);
                printf("child return value[%d] \n", WEXITSTATUS(state));
            }
        } while( child == 0 ); // No child remained

        printf("After waitpid\n");
        sleep(2);
        system("ps -ef | grep waitpid");
    }
    printf("data : %d \n", data);

    if( pid > 0 )
    {
        sleep(5); 
        printf("Final waitpid\n");
        sleep(2);
        system("ps -ef | grep waitpid");
    }
    return 0;
}