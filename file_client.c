/* file_client.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>

#define FILE_BUFSIZE 1000

void error_handling(char *message);

int main( int argc, char **argv)
{
	int clnt_sock;
	struct sockaddr_in serv_addr;
	char buf[FILE_BUFSIZE+1];
	int read_len;
	
	if(argc != 4)
	{
		printf("Usage : %s <IP> <port> <filename>\n", argv[0]);
		exit(1);
	}
    
    int fd = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC);
    if ( fd == -1 )
        error_handling(" open() error !!!");

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

	printf("connect() : request ip [%s] port[%s] file[%s]\n", argv[1], argv[2], argv[3]);

	sleep(1);
	// 1st write for requesting file name
	memset(buf, 0x00, sizeof(buf));
	memcpy(buf, "file_name : ", 12);
	memcpy(buf+12, argv[3], strlen(argv[3]));
	
	printf("write client socket [%s]\n", buf);
	write(clnt_sock, buf, strlen(buf));
	sleep(2);
	//
	while( (read_len = read(clnt_sock, buf, FILE_BUFSIZE)) != 0 ) 
	{
		write(fd, buf, read_len);
	}

	write(clnt_sock,"Thank you\n", 10);
	close(fd);
	close(clnt_sock);

	return 0;
}

void error_handling(char *message)
{
	fputs(message,stderr); 
	fputc('\n', stderr);
	exit(1);
}
