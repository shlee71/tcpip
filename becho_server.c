/* becho_server.c
   add incoming firewall IP & port ( CENTOS )
   1. sudo firewall-cmd --zone=public --permanent --add-rich-rule='rule family="ipv4" source address="192.168.110.130/32" port protocol="udp" port="8888" accept'  
   2. sudo cat /etc/firewalld/zones/public.xml
   3. sudo firewall-cmd --reload

   add incoming firewall IP & port ( UBUNTU )
   1. sudo ufw allow from 192.168.110.128 to any port 8888 proto udp
 
   network check
   1. sudo netstat -tulpn
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
	int serv_sock;
	int clnt_sock;
	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;

    int clnt_addr_size, str_len;
    char message[BUFSIZE+1];
    int num = 0;
	
	if ( argc != 2){
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	
	serv_sock = socket(PF_INET,SOCK_DGRAM, 0);
	if(serv_sock == -1)
		error_handling("socket() error");
	
    printf("socket() created \n");

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port        = htons(atoi(argv[1]));

    if( bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1 )
		error_handling("bind() error");
    
    sleep(5);
    
    while(1)
    {
        clnt_addr_size = sizeof(clnt_addr);
        memset(message, 0x00, sizeof(message));

        str_len = recvfrom(serv_sock, message, BUFSIZE, 0, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
        printf("receive message from client : no[%d] [%s]\n", num++, message);
        sendto(serv_sock, message, str_len, 0, (struct sockaddr *)&clnt_addr, clnt_addr_size );

        // UDP Socket server close message between client and server
        if ( memcmp(message,"Bye", 3) == 0 )
            break;
    }

    close(serv_sock);
    return 0;
}

void error_handling(char *message)
{
	fputs(message,stderr); 
	fputc('\n', stderr);
	exit(1);
}
