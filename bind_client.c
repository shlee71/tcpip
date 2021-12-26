/* bind_client.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

void error_handling(char *message);

int main( int argc, char **argv)
{
	int clnt_sock;
	struct sockaddr_in serv_addr;
    char msg1[] = "Hello there.";
    char msg2[] = "This is Hoya. Are you happy ?";
	char message[10+1];
	int str_len;
	
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
    write(clnt_sock, msg1, strlen(msg1));
    write(clnt_sock, msg2, strlen(msg2));
    sleep(10);

    for (int i= 0; i < 5 ; i++)
    {
        memset(message, 0x00, sizeof(message));
	    str_len = read(clnt_sock, message, sizeof(message)-1);
	    if ( str_len == -1 )
		    error_handling("read() error !!!!");

  	    printf("read() [%d]  : message from server : %s\n", str_len, message);
    }
    close(clnt_sock);
	
	return 0;
}

void error_handling(char *message)
{
	fputs(message,stderr); 
	fputc('\n', stderr);
	exit(1);
}
