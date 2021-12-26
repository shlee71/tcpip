/* file_server.c
   add incoming firewall IP & port ( CENTOS )
   1. sudo firewall-cmd --zone=public --permanent --add-rich-rule='rule family="ipv4" source address="192.168.110.130/32" port protocol="tcp" port="9999" accept'  
   2. sudo cat /etc/firewalld/zones/public.xml
   3. sudo firewall-cmd --reload

   add incoming firewall IP & port ( UBUNTU )
   1. sudo ufw allow from 192.168.110.128 to any port 9999
 
   network check
   1. sudo netstat -tulpn
   2. sudo lsof -i -P -n | grep LISTEN
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>

#define FILE_BUFSIZE 1000
#define BUFSIZE 100

void error_handling(char *message);

int main( int argc, char **argv)
{
	int serv_sock;
	int clnt_sock;
	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	
	int clnt_addr_size, read_len;
    char buf[FILE_BUFSIZE+1];
	char file_name[BUFSIZE+1];
	
	if ( argc != 2){
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	serv_sock = socket(PF_INET,SOCK_STREAM, 0);
	if(serv_sock == -1)
		error_handling("socket() error");
	
    printf("socket() created \n");

    // add bind option for avoiding error of direct reconnection after the end sock close 
    // Linux system return error while TCP is tried reconnection after being closed in xx secound
    // for the system integraty
    socklen_t opt = 1;
    setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    // the end of error avoiding

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

    //while(1)
    //{
        clnt_addr_size = sizeof(clnt_addr);
        clnt_sock =accept(serv_sock, (struct sockaddr*) &clnt_addr, &clnt_addr_size);
        if( clnt_sock == -1)
            error_handling("accep t() error"); 

        printf("accept() : message from client ip[%s] is received / created client socket[%d] \n\n", 
                        inet_ntoa(clnt_addr.sin_addr), clnt_sock);

        // 1st read for file name to send
        read_len = read(clnt_sock, buf, BUFSIZE);

        printf("requested message from client[%s]\n", buf);

        if ( memcmp(buf,"file_name : ", 12) == 0 )
        {
            memset(file_name, 0x00, sizeof(file_name));
            strncpy(file_name, buf + 12, read_len -12 ); 
            printf("requested file name from server [%s]\n", file_name);
        }
        else
        {
            error_handling("1st read() requested file name error"); 
        }

        int fd = open(file_name, O_RDONLY);
        if ( fd == -1 )
            error_handling(" open() error !!!");

        while( (read_len = read(fd, buf, FILE_BUFSIZE)) != 0)
        {
            write(clnt_sock, buf, read_len);
        }

        if(shutdown(clnt_sock, SHUT_WR) != 0)
            error_handling("shutdown() error");

        read_len = read(clnt_sock, buf, BUFSIZE);
        write(1, buf, read_len);

        close(fd);
        close(clnt_sock);
    //}
	close(serv_sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message,stderr); 
	fputc('\n', stderr);
	exit(1);
}
