/*
 * dup2.c
 */

#include <stdio.h>
#include <unistd.h>

int main(void)
{
    int fd, state;
    fd = dup(1); // 1 : standard output
    printf("Copied file descriptor number : %d \n", fd);
    write(fd, "1st Write with copied file descriptor \n", 40);

    state = close(1);
    if ( state == -1)
        printf("Error close(1) \n");
    
    write(fd, "2nd Write with copied file descriptor \n", 40);
    return 0;
}