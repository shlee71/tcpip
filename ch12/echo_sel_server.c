/* echo_sel_server.c
   add incoming firewall IP & port ( CENTOS )
   1. sudo firewall-cmd --zone=public --permanent --add-rich-rule='rule family="ipv4" source address="192.168.110.130/32" port protocol="tcp" port="9999" accept'  
   2. sudo cat /etc/firewalld/zones/public.xml
   3. sudo firewall-cmd --reload

   add incoming firewall IP & port ( UBUNTU )
   1. sudo ufw allow from 192.168.110.128 to any port 9999
 
   network check
   1. sudo netstat -tulpn

   open file descriptor
   1. lsof -p <pid>  : ex. lsof -p 28290

   typedef struct
   {
       long fds_bits[1024 / 64];
   } fd_set;
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <sys/select.h>

#define BUFSIZE 100

void error_handling(char *message);

int main( int argc, char **argv)
{
	int serv_sock;
	struct sockaddr_in serv_addr;
	fd_set readfds, tempfds;
    int fds_max;
	
	int str_len;
	char message[BUFSIZE+1];
    struct timeval timeout;
	
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

    FD_ZERO(&readfds);
    FD_SET(serv_sock, &readfds);
    fds_max = serv_sock;

    // default fds 
    // 0 : system input  
    // 1 : system output 
    // 2 : system signal 
    // 3 : server socket 
    printf("fds_max =[%d]\n", fds_max);

    while(1)
    {
        int fds, str_len;
        int clnt_sock, clnt_len;
        struct sockaddr_in clnt_addr;

        tempfds = readfds;
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;

        if(select(fds_max+1, &tempfds, 0, 0,&timeout) == -1)
            error_handling("select() error");
        
        if(select(fds_max+1, &tempfds, 0, 0,&timeout) == 0)
        {
            printf("Timeout occured !!! : [%d] \n", fds_max);
            continue;
        }

        for( fds = 0; fds < fds_max+1; fds++)
        {
            printf("loop fds[%d], FD_ISSET[%d] [%4x] \n", fds, FD_ISSET(fds, &tempfds), tempfds);
            if (FD_ISSET(fds, &tempfds))
            {
                if(fds == serv_sock)
                {
                    clnt_len = sizeof(clnt_addr);
                    clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_len);

                    FD_SET(clnt_sock,&readfds);
                    if (fds_max < clnt_sock)
                        fds_max = clnt_sock;
                    printf("accept() : message from client ip[%s] is received / created client socket[%d], fds_max[%d] \n\n", 
                    inet_ntoa(clnt_addr.sin_addr), clnt_sock, fds_max);
                }
                else 
                {
                    str_len = read(fds, message, BUFSIZE);
                    if( str_len == 0) // client close socket
                    {
                        FD_CLR(fds, &readfds);
                        close(fds);
                        printf("client fd[%d] closed : ip[%s], socket[%d]\n",fds, inet_ntoa(clnt_addr.sin_addr), clnt_sock);
                    } 
                    else 
                    {
                        write(fds, message, str_len);
                        printf("returned message to client : %s\n", message);
                    }

                    memset(message, 0x00, BUFSIZE);
                }
            }// if ( FD_ISSET(fd, &tempfds))
        }// for( fd = 0; fd < fds_max +1; fd++)
    } // while(1)

 	close(serv_sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message,stderr); 
	fputc('\n', stderr);
	exit(1);
}
