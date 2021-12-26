/*
 * dup1.c
 */

#include <stdio.h>
#include <unistd.h>

int main(void)
{
    int fd;
    fd = dup(1); // 1 : standard output
    printf("Copied file descriptor number : %d \n", fd);
    write(fd, "Write with copied file descriptor \n", 36);

    return 0;
}