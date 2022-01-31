/* file_get_server.c
   add incoming firewall IP & port ( CENTOS )
   1. sudo firewall-cmd --zone=public --permanent --add-rich-rule='rule family="ipv4" source address="192.168.110.130/32" port protocol="tcp" port="9999" accept'  
   2. sudo cat /etc/firewalld/zones/public.xml
   3. sudo firewall-cmd --reload

   add incoming firewall IP & port ( UBUNTU )
   1. sudo ufw allow from 192.168.110.128 to any port 9999
 
   network check
   1. sudo netstat -tulpn
   2. sudo lsof -i -P -n | grep LISTEN

   execut method
   1. file_get_server <port>

   Received File write Position : 
   Make the diretory under the root and change directory access mode to 666
   1. /CMS/<ID>/<file_name>
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
    int wait_timeout_cnt;

    char buf[FILE_BUFSIZE+1];
	char file_name[BUFSIZE+1];
    char eof_msg[BUFSIZE+1];
	
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

    while(1)
    {
        clnt_addr_size = sizeof(clnt_addr);
        clnt_sock =accept(serv_sock, (struct sockaddr*) &clnt_addr, &clnt_addr_size);
        if( clnt_sock == -1)
            error_handling("accep t() error"); 

        printf("accept() : message from client ip[%s] is received / created client socket[%d] \n\n", 
                        inet_ntoa(clnt_addr.sin_addr), clnt_sock);

        // 1st read for file name to send
        read_len = read(clnt_sock, buf, BUFSIZE);

        printf("requested message from client[%s]/[%d]\n", buf, read_len);

        memset(eof_msg, 0x00, sizeof(eof_msg));
        memcpy(eof_msg,"EOF-",4);
        memcpy(eof_msg+4,buf, read_len );

        memset(file_name, 0x00, sizeof(file_name));
        strncpy(file_name, "/CMS/", 5);
        //ID + File_name
        strncpy(file_name+5 ,buf   , read_len ); 
        printf("requested file name from server [%s]\n", file_name);
               
        int fd = open(file_name, O_RDWR | O_CREAT | O_TRUNC, 0666);
        if ( fd == -1 )
            error_handling(" open() error !!!");
        
        sleep(1);
        write(clnt_sock, "Ready for receiving OK", 22);

        sleep(1);
        printf("ready for receiving file \n");

        wait_timeout_cnt = 0;

        while(1)
        {
            read_len = read(clnt_sock, buf, FILE_BUFSIZE);
            if ( read_len > 0)
            {
                if ( memcmp(buf, eof_msg, read_len) != 0)
                {
                    printf("File receiving ...\n");
                    write(fd, buf, read_len);
                }
                else
                {
                    break;
                }
            }
            else
            {
                wait_timeout_cnt++;
                sleep(1);
            }

            if ( wait_timeout_cnt > 10 )
                error_handling("Receving Timeout !!!");
        }

        printf("File recevied success [%s]\n", eof_msg);

        if(shutdown(clnt_sock, SHUT_RDWR) != 0)
            error_handling("shutdown() error");

        read_len = read(clnt_sock, buf, BUFSIZE);
        write(1, buf, read_len);

        close(fd);
        close(clnt_sock);
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
