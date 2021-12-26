#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>

#define BUFSIZE 100

void error_handling(char *message);

int main(void)
{
	int fdes1, fdes2, fdes3;
	
	fdes1= socket(PF_INET, SOCK_STREAM, 0);
	fdes2= open  ("test.dat", O_CREAT);
	fdes3= socket(PF_INET, SOCK_DGRAM , 0);
	
	printf("1st file descripter no : %d\n", fdes1);
	printf("2nd file descripter no : %d\n", fdes2);
	printf("3rd file descripter no : %d\n", fdes3);
	
	close(fdes1);
	close(fdes2);
	close(fdes3);
	
	return 0;
}

void error_handling(char *message)
{
	fputs(message,stderr); 
	fputc('\n', stderr);
	exit(1);
}
