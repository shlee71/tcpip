/* 
 * fork.c
 * 1. ps -u  | grep fork
 * 2. ps -ef | grep fork
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

    if(pid == 0 ) // child process
    {
        
        data += 10;
    }
    else          // parent process
    {
        data -= 10;
        child = wait(&state);
        printf("child process id [%d] \n", child);
        printf("return value     [%d] \n", WEXITSTATUS(state));
        sleep(10);
    }
    printf("data : %d \n", data);

    return 0;

}