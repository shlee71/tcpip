/*
    pipe3.c
    good method both direction ( parent <-> child ) using 2 pipe.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define BUFSIZE 30

int main(int argc, char **argv)
{
    int fd1[2], fd2[2];
    char buffer[BUFSIZE+1];
    pid_t pid;
    int state;
    
    if( pipe(fd1) == -1 || pipe(fd2) == -1) {
        puts("pip() error");
        exit(1);
    }

    pid = fork();

    if( pid == -1) {
        puts("fork() error");
        exit(1);
    } else if ( pid == 0 ) {
        // fd1 : from child to parent  
        write(fd1[1],"Good!", 6);
        // fd2 : from parent to child
        read(fd2[0], buffer, BUFSIZE);
        printf("Child  process print [%s] \n",buffer);
    } else {
        read( fd1[0], buffer, BUFSIZE);
        printf("Parent process print [%s] \n",buffer);
        write(fd2[1],"Really Good ?", 14);
        sleep(1);
    }

    return 0;
}
