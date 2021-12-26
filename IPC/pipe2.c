/*
    pipe2.c
    Not good method both direction ( parent <-> child ) using 1 pipe.
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
        write(fd[1],"Good!", 6);
        sleep(2);
        read(fd[0], buffer, BUFSIZE);
        printf("Child  process print [%s] \n",buffer);
    } else {
        read( fd[0], buffer, BUFSIZE);
        printf("Parent process print [%s] \n",buffer);
        write(fd[1],"Really Good ?", 14);
        sleep(3);
    }

    return 0;
}
