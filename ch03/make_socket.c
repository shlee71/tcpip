#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

void error_handling(char *message);

int main(void)
{
	int tcp_socket;
	int udp_socket;
	
	tcp_socket = socket(PF_INET,SOCK_STREAM, IPPROTO_TCP);
	if(tcp_socket == -1)
		error_handling("socket() error");
	
	udp_socket = socket(PF_INET,SOCK_DGRAM , 0);
	if(tcp_socket == -1)
		error_handling("socket() error");
	
	printf("Created TCP socket fd : %d\n", tcp_socket);
	printf("Created UDP socket fd : %d\n", udp_socket);
	
	close(tcp_socket);
	close(udp_socket);
	return 0;
}

void error_handling(char *message)
{
	fputs(message,stderr); 
	fputc('\n', stderr);
	exit(1);
}
