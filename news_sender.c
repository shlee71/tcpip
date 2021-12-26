/* helloworld_client.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define BUFSIZE 100
#define TTL 64

void error_handling(char *message);

int main( int argc, char **argv)
{
	int send_sock;
	struct sockaddr_in multi_addr;
    int state;
    int multi_TTL = TTL;
    FILE *fp;
    char buf[BUFSIZE];
	
	if(argc != 3)
	{
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}
	
	send_sock = socket(PF_INET,SOCK_DGRAM, 0);
	if(send_sock == -1)
		error_handling("socket() error");
    
    printf("Multicast socket() created \n");

	memset(&multi_addr, 0, sizeof(multi_addr));
	multi_addr.sin_family = AF_INET;
	multi_addr.sin_addr.s_addr = inet_addr(argv[1]);
	multi_addr.sin_port        = htons(atoi(argv[2]));
	
	state = setsockopt(send_sock, IPPROTO_IP, IP_MULTICAST_TTL, (void*)&multi_TTL, sizeof(multi_TTL) );

    if(state)
		error_handling("setsockopt() error");
	
    printf("setsockopt : request ip [%s] port[%s] ttl[%d]\n", argv[1], argv[2], multi_TTL);

    if((fp=fopen("NEWS.txt","r"))== NULL)
        error_handling("fopen() error");

    while(!feof(fp)) 
    {
        fgets(buf, BUFSIZE, fp);
        sendto(send_sock, buf, strlen(buf), 0, (struct sockaddr *)&multi_addr, sizeof(multi_addr));
        sleep(2);
    }

    close(send_sock);
	
	return 0;
}

void error_handling(char *message)
{
	fputs(message,stderr); 
	fputc('\n', stderr);
	exit(1);
}
