/* uecho_client2.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define BUFSIZE 100

void error_handling(char *message);

int main( int argc, char **argv)
{
	int clnt_sock;
	struct sockaddr_in serv_addr;
    struct sockaddr_in from_addr;
	char message[100];
	int str_len, addr_size;
	
	if(argc != 3)
	{
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}
	
	clnt_sock = socket(PF_INET,SOCK_DGRAM, 0);
	if(clnt_sock == -1)
		error_handling("socket() error");
    
    printf("socket() : socket created \n");

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port        = htons(atoi(argv[2]));

    printf("to send : request ip [%s] port[%s] \n", argv[1], argv[2]);
    connect(clnt_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    while(1) {
        // Message request from console
        fputs("Please Input sending message ( q to quit )", stdout);
        fgets(message,BUFSIZE, stdin);

        if(strncmp(message,"q",1) == 0)
            break;

        // send message to server
        write(clnt_sock, message, strlen(message));
        
        // receive message length save
        int addr_size = sizeof(from_addr); 
        str_len = read (clnt_sock, message, BUFSIZE);
        //receive message print
        message[str_len] = 0;
        printf("Received message from server : len[%d] %s \n", str_len, message);
        memset(message, 0x00, sizeof(message));
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
