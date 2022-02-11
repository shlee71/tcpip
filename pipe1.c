/*
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define BUFSIZE 30

int main(int argc, char **argv)
{
    int fd[2]; // 0 : read pipe, 1 : write pipe
    char buffer[BUFSIZE+1];
    pid_t pid;
    int state;

    state = pipe(fd);
    if( state == -1) {
        puts("pip() error");
        exit(1);
    }

    pid = fork();

    if( pid == -1) {
        puts("fork() error");
        exit(1);
    } else if ( pid == 0 ) {
        printf("child process write message ==> ");
        memset(buffer, 0, sizeof(buffer));
        memcpy(buffer,"Good Morning!!", BUFSIZE); 
        write(fd[1], buffer, strlen(buffer));
        printf("%s\n", buffer);
    } else {
        sleep(2);
        printf("parent process read message ==> ");
        read( fd[0], buffer, BUFSIZE);
        puts(buffer);
    }

    return 0;
}
