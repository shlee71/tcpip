#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void error_handling(char *message);

int main(void)
{
	int fildes;
	char buf[] ="Lets' go together !\n";
	
	fildes=open("data.txt", O_CREAT|O_WRONLY|O_TRUNC);
	if(fildes == -1)
		error_handling("open() error!");
	
	printf("Created file descripdor is %d\n", fildes);
	
	if(write(fildes, buf, sizeof(buf)) == -1)
		error_handling("write() error !!!");
	
	close(fildes);
	return 0;
}

void error_handling(char *message)
{
	fputs(message,stderr); 
	fputc('\n', stderr);
	exit(1);
}
