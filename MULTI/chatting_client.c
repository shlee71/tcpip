/*
	Thread 통해 recv 기능을 분리해, blocking 을 막기위한 client example
	 gcc chatting_client.c -o chatting_client -lpthread
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>

#define BUFSIZE 1024
void *recv_thread(void *sock);
void error_handling(char *message);

int main( int argc, char **argv)
{
	int clnt_sock;
	struct sockaddr_in serv_addr;
	char message[BUFSIZE];
	int str_len;
	pthread_t tid;
	
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
	
    printf("connect() : request ip [%s] port[%s] sock[%d] \n", argv[1], argv[2], clnt_sock);
	pthread_create(&tid, NULL, (void *)recv_thread, (void *)(&clnt_sock));

    while(1) {
        // Message request from console
		memset(message,0x00, BUFSIZE);
        fputs("Please Input sending message ( q to quit )\n", stdout);
        fgets(message,BUFSIZE, stdin);

        if(strncmp(message,"q",1) == 0)
            break;

        write(clnt_sock, message, strlen(message));
        //write(1, message, strlen(message));

        memset(message, 0x00, sizeof(BUFSIZE));
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

void *recv_thread(void *sock)
{
	int msg_len;
	int clnt_sock = *(int *)sock;
	char msg[BUFSIZE];

	while(1)
	{
		// read real-time receiving data from server
		msg_len = read(clnt_sock, msg, sizeof(msg));
		msg[msg_len] = '\0';

		if( msg_len <= 0)
		    break;
		else
			printf("[recv] %s\n", msg);
		
		bzero(msg, sizeof(msg));
	}

	close(clnt_sock);
	printf("\nclose client sock ==> [%d] \n", clnt_sock);
}