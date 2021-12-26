/* 
 * zombie_handler.c
 * struct sigaction
 * {
 *      void(*sa_handler)(int);
 *      sigset_t sa_mask;
 *      int sa_flags;
 * }
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

void z_handler(int sig);
int main(int argc, char **argv)
{
    pid_t pid;
    int state, i;

    struct sigaction act;
    act.sa_handler = z_handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    state = sigaction(SIGCHLD, &act, 0);
    if (state != 0)
    {
        puts("sigactin() error");
        exit(1);
    }

    pid = fork();
    if( pid == 0 )
    {
        printf("Child process PID : %d \n", getpid());
        exit(1);
    }
    else 
    {
        sleep(3);
    }

    return 0;
}
void z_handler(int sig)
{
    pid_t pid;
    int rtn;

    while ( pid = waitpid(-1, &rtn, WNOHANG) > 0)
    {
        printf("dropped zombie pid : %d\n", pid);
        printf("returned value     : %d \n\n", WEXITSTATUS(rtn));
    }
}