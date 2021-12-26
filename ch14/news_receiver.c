/* news_receiver.c
 */
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
	int recv_sock;
	struct sockaddr_in recv_addr;

    int state, str_len;
	char buf[BUFSIZE];
    struct ip_mreq join_addr;
	
	if ( argc != 3){
		printf("Usage : %s <GroupIP> <port>\n", argv[0]);
		exit(1);
	}
	
	recv_sock = socket(PF_INET,SOCK_DGRAM, 0);
	if(recv_sock == -1)
		error_handling("socket() error");
	
    printf("socket() created \n");

	memset(&recv_addr, 0, sizeof(recv_addr));
	recv_addr.sin_family = AF_INET;
	recv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	recv_addr.sin_port        = htons(atoi(argv[2]));

    if( bind(recv_sock, (struct sockaddr*) &recv_addr, sizeof(recv_addr)) == -1 )
		error_handling("bind() error");

    printf("bind() processed ip[%s] , port[%d]  \n", inet_ntoa(recv_addr.sin_addr) , atoi(argv[2]));

    join_addr.imr_multiaddr.s_addr=inet_addr(argv[1]);
    join_addr.imr_interface.s_addr=htonl(INADDR_ANY);

    state=setsockopt(recv_sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void *)&join_addr, sizeof(join_addr));
    if(state)
        error_handling("setsockopt() error");

    printf("setsockopt() processed \n");
    
    while(1)
    {
        memset(buf, 0x00, sizeof(buf));

        str_len = recvfrom(recv_sock, buf, BUFSIZE-1, 0, NULL, 0);
        if ( str_len < 0) break;
        buf[str_len] = 0;
        fputs(buf, stdout);
    }

    close(recv_sock);
    return 0;
}

void error_handling(char *message)
{
	fputs(message,stderr); 
	fputc('\n', stderr);
	exit(1);
}
