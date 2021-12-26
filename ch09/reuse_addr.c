/* reuse_addr.c
   add incoming firewall IP & port ( CENTOS )
   1. sudo firewall-cmd --zone=public --permanent --add-rich-rule='rule family="ipv4" source address="192.168.110.130/32" port protocol="tcp" port="9999" accept'  
   2. sudo cat /etc/firewalld/zones/public.xml
   3. sudo firewall-cmd --reload

   add incoming firewall IP & port ( UBUNTU )
   1. sudo ufw allow from 192.168.110.128 to any port 9999
 
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
#include <netinet/tcp.h>

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
	
	if ( argc != 2){
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	
	serv_sock = socket(PF_INET,SOCK_STREAM, 0);
	if(serv_sock == -1)
		error_handling("socket() error");
	
    printf("socket() created \n");

    // TIME_NOWAIT
    socklen_t opt = 1;
    setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    // Nagle algoritm off : netinet/tcp.h 추가 필수
    socklen_t opt_val = 1;
    setsockopt(serv_sock, IPPROTO_TCP, TCP_NODELAY, &opt_val, sizeof(opt_val));

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port        = htons(atoi(argv[1]));
	
	if( bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1 )
		error_handling("bind() error");
	
    printf("bind() is lined up \n");

	if( listen(serv_sock, 5) == -1)
		error_handling("listen() error");
	
    printf("listen() : now it's ready\n");

	clnt_addr_size = sizeof(clnt_addr);
   
    clnt_sock =accept(serv_sock, (struct sockaddr*) &clnt_addr, &clnt_addr_size);
    if( clnt_sock == -1)
        error_handling("accep t() error"); 

    printf("accept() : message from client ip[%s] is received / created client socket[%d] \n\n", 
                    inet_ntoa(clnt_addr.sin_addr), clnt_sock);

    while( read(clnt_sock, message, BUFSIZE) != 0)
    {
        // str_len = read(clnt_sock, message, BUFSIZE) 값이 1로 Setting 됨.
        // 문제해결전까자 strlen 으로 수정
        str_len = strlen(message); 
        write(clnt_sock, message, str_len);
        write(1, message, str_len);
        memset(message, 0x00, str_len);
    }

    close(clnt_sock);
	close(serv_sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message,stderr); 
	fputc('\n', stderr);
	exit(1);
}
