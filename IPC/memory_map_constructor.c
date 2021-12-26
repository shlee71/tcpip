#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

#define MAP_SIZE 4096

int main(int argc, char **argv)
{
    int fd;
    int *pmmap;
    int i;

    //Memory map file open
    fd = open("MMAP.mm", O_RDWR|O_CREAT, 0666);
    if(fd < 0)
    {
        perror("open");
        exit(1);
    }
    pmmap = (int *)malloc(MAP_SIZE);
    //ftruncate(fd, MAP_SIZE); 
    //MAP_SHARED : share map with other process
    //void *mmap(void *addr, size_t length, int prot, int flags,int fd, off_t offset);
    // addr : to be allocated memory address, NULL is usually used for kernel allocation
    // prot : Memory procection Mode PROT_READ, PROT_WRITE, PROT_NONE
    // flags : MAP_SHARED vs MAP_PRIVATE ( copied memory )
    pmmap = (int *)mmap(0, MAP_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if(pmmap == NULL)
    {
        perror("mmap");
        exit(1);
    }

    for(i = 0; i < 100; i++)
    {
        pmmap[i] = i*i; 
    }
    pmmap[i+1] = -1; 
    getchar();
    munmap(pmmap, MAP_SIZE);
    close(fd);
    return 0;
}