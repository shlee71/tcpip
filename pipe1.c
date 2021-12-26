/*
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define BUFSIZE 30

int main(int argc, char **argv)
{
    int fd[2];
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
        write(fd[1],"Good\n", 6);
    } else {
        read( fd[0], buffer, BUFSIZE);
        puts(buffer);
    }

    return 0;
}
