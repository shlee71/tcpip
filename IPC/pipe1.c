/*
 * pipe example between parent and child
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define BUFSIZE 30

int main(int argc, char **argv)
{
    // define 2 file description for read / write 
    int fd[2];
    char buffer[BUFSIZE+1];
    pid_t pid;
    int state;
    int nbytes;

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
	// child prcess : close read fd and send messages 
        close(fd[0]);
	    strncpy(buffer, "Message from child process !!!", BUFSIZE);
        write(fd[1], buffer,strlen(buffer));
        printf("Child sent [%d] bytes string [%s] to parent process\n\n", strlen(buffer), buffer);
    } else {
	// parent process : close write fd and receive messages
	    close(fd[1]);
        nbytes = read( fd[0], buffer, BUFSIZE);
	    printf("Parent received [%d] bytes string [%s] from child process\n\n", nbytes, buffer);
    }

    return 0;
}
