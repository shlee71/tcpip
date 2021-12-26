#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#define MAP_SIZE 4096

int main(int argc, char **argv)
{
    int fd;
    int i=0;
    char *file = NULL;
    char *linebuf;
    int *maped;
    int flag = PROT_WRITE | PROT_READ;

    if ((fd = open("MMAP.mm", O_RDWR, 0666)) < 0)
    {
        perror("File Open Error");
        exit(1);
    }

    // mmap�� �̿��ؼ� ���� ������ �޸𸮿� ������Ų��. 
    // file�� ������ �ּҸ� ����Ű��, file�� �̿��ؼ� �ʿ��� �۾���  
    // �ϸ� �ȴ�.  
    maped = (int *) mmap(0, MAP_SIZE, flag, MAP_SHARED, fd, 0);
    if ( maped == NULL)
    {
        perror("mmap error");
        exit(1);
    }

    while(1)
    {
        if (maped[i] == -1) break;
        printf("Memory Map data[%d] -> %d\n",i, maped[i]);
        i++;
    }
    close(fd);
}