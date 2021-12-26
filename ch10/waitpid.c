/* 
 * waitpid.c
 */

#include <stdio.h>
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
            child=waitpid(-1, &state, WNOHANG);
        } while( child == 0 );

        printf("child process id  [%d] \n", child);
        printf("child return value[%d] \n", WEXITSTATUS(state));
        sleep(20);
    }
    printf("data : %d \n", data);

    return 0;
}