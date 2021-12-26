/* sep_client.c */
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
	char buf[BUFSIZE];

    FILE *rstrm;
    FILE *wstrm;
	
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

    rstrm = fdopen(clnt_sock, "r");
    wstrm = fdopen(clnt_sock, "w");

    while(1) {
        // Message request from console
		memset(buf,0x00, BUFSIZE);
        if(fgets(buf, sizeof(buf), rstrm) == NULL) break;
        fputs(buf,stdout);
        fflush(stdout);
        memset(buf, 0x00, sizeof(BUFSIZE));
    }

    // Final message send
    fputs("From Client : Trhank you\n", wstrm);
    fclose(wstrm);
	
	return 0;
}

void error_handling(char *message)
{
	fputs(message,stderr); 
	fputc('\n', stderr);
	exit(1);
}
