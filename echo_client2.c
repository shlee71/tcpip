/* helloworld_client.c */
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
	char message[BUFSIZE+1];
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

    while(1) 
    {
        // Message request from console
        memset(message, 0x00, sizeof(message));
        fputs("Please Input sending message ( q to quit )", stdout);
        fgets(message, BUFSIZE, stdin);

        if(strncmp(message,"q",1) == 0)
            break;

        write(clnt_sock, message, strlen(message));
        write(1, message, strlen(message));
        
        // send messave length save
        int send_len = strlen(message); 

        //received mssage print
        message[BUFSIZE+1] = 0;
        
        int recv_num = 0;
        int recv_len = 0;

        //read message until expect message length being received.  
        for ( recv_len = 0 ; recv_len < send_len; )
        {
            recv_num = read(clnt_sock, &message[recv_len], BUFSIZE);
            if ( recv_num == -1 ) 
                error_handling("read() error !!!");
            
            recv_len += recv_num;
        }
        
        printf("Received message from server : %s \n", message);

        memset(message, 0x00, sizeof(BUFSIZE+1));
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
