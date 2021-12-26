/* news_receiver.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

void error_handling(char *message);

int main( void)
{
    int filedes;
    FILE *fp;

    /* file create system function */
    filedes = open("data.dat",O_WRONLY|O_CREAT|O_TRUNC);
    if( filedes == -1)
        error_handling("file open() error");
    
    /* create file pointer with file descriptor */
    fp=fdopen(filedes,"W");

    fputs("Network C programming \n", fp);
    fclose(fp);

    return 0;

}
void error_handling(char *message)
{
	fputs(message,stderr); 
	fputc('\n', stderr);
	exit(1);
}

