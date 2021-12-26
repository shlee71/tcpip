/* 
 * fork.c
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main( int argc, char **argv)
{
    pid_t pid;
    int data = 10;

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
        sleep(10);
    }
    printf("data : %d \n", data);

    return 0;

}