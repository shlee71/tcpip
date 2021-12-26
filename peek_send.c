/* oob_send.c */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUFSIZE 100

void error_handling(char *message);

int main( int argc, char **argv)
{
	int clnt_sock;
	struct sockaddr_in serv_addr;
	
	if(argc != 3)
	{
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}
	
	clnt_sock = socket(PF_INET,SOCK_STREAM, 0);
	if(clnt_sock == -1)
		error_handling("socket() error");
    
    printf("socket() : socket created \n");

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port        = htons(atoi(argv[2]));
	
	if( connect(clnt_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1 )
		error_handling("connect() error");
	
    printf("connect() : request ip [%s] port[%s] \n", argv[1], argv[2]);

    write(clnt_sock, "123", strlen("123"));
        
    sleep(1);

    close(clnt_sock);
	
	return 0;
}

void error_handling(char *message)
{
	fputs(message,stderr); 
	fputc('\n', stderr);
	exit(1);
}
