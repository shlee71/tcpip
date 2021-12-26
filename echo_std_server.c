/* echo_std_server.c
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

#define BUFSIZE 1024

void error_handling(char *message);

int main( int argc, char **argv)
{
	int serv_sock;
	int clnt_sock;
	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;

    FILE* readfp;
    FILE* writefp;
	
	int clnt_addr_size, str_len;
	char message[BUFSIZE];
	
	if ( argc != 2){
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	
	serv_sock = socket(PF_INET,SOCK_STREAM, 0);
	if(serv_sock == -1)
		error_handling("socket() error");
	
    printf("socket() created \n");

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

    /* file create system function */
    readfp  = fdopen(clnt_sock, "r");
    writefp = fdopen(clnt_sock, "w");

    while(!feof(readfp))
    {
		memset(message, 0x00, sizeof(message));
        fgets(message,BUFSIZE, readfp);
		printf("Server recv message : %s\n", message);
		// Socket close condition
		if ( strlen(message) == 0 ) break;
	 
        fputs(message, writefp);
        fflush(writefp);
        printf("Server echo message : %s\n", message);
    }

    close(clnt_sock);
	close(serv_sock);
    
    fclose(readfp);
    fclose(writefp);
	return 0;
}

void error_handling(char *message)
{
	fputs(message,stderr); 
	fputc('\n', stderr);
	exit(1);
}
