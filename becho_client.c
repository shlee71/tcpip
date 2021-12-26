/* becho_client.c */
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
	int str_len, addr_size, i;

    char msg1[] = "Good bye everybody.";
    char msg2[] = "I've got to go.";
    char msg3[] = "Gotta leave you all behind and face the truth.";
	
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

    // send message to server
    sendto(clnt_sock, msg1, strlen(msg1), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    sendto(clnt_sock, msg2, strlen(msg2), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    sendto(clnt_sock, msg3, strlen(msg3), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    for ( i =0; i< 3; i++) 
    {
       
        // receive message length save
        addr_size = sizeof(from_addr); 
        str_len = recvfrom(clnt_sock, message, BUFSIZE, 0, (struct sockaddr *)&from_addr, &addr_size);
        //receive message print
        message[str_len] = 0;
        printf("Received message from server : no[%d] len[%d] [%s] \n", i, str_len, message);
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
