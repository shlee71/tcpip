/* 
 * waitpid.c
 * zombie process »Æ¿Œ
 * 1. ps -u | grep  waitpid
 * 2. ps -ef | grep waitpid
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
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
        do {
            sleep(1);
            printf("Count down [%d]\n",count_down--);
            system("ps -ef | grep zombie_waitpid");
            child=waitpid(-1, &state, WNOHANG);
        } while( child == 0 ); // No child remained

        printf("child process id  [%d] \n", child);
        printf("child return value[%d] \n", WEXITSTATUS(state));
        system("ps -ef | grep zombie_waitpid");
        sleep(1);
    }
    printf("data : %d \n", data);

    return 0;
}