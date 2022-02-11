/* echo_sel_server.c
   add incoming firewall IP & port ( CENTOS )
   1. sudo firewall-cmd --zone=public --permanent --add-rich-rule='rule family="ipv4" source address="192.168.110.130/32" port protocol="tcp" port="9999" accept'  
   2. sudo cat /etc/firewalld/zones/public.xml
   3. sudo firewall-cmd --reload

   add incoming firewall IP & port ( UBUNTU )
   1. sudo ufw allow from 192.168.110.128 to any port 9999
 
   network check
   1. sudo netstat -tulp

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
int printbin(unsigned char *prt_str, void *buf, int len);

int main( int argc, char **argv)
{
	int serv_sock;
	struct sockaddr_in serv_addr;

	fd_set readfds, tempfds;
    union test { unsigned char x[4]; fd_set chkfds; } rd;

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
	
    printf("server socket() created \n");

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

    //Server socket connection request flag set
    FD_ZERO(&readfds);
    FD_SET(serv_sock, &readfds);

    //Search fd count set
    fds_max = serv_sock;

    char prt_str[sizeof(readfds)*8+1];
    memset(prt_str, 0x00, sizeof(prt_str));
    printbin(prt_str, &readfds, sizeof(readfds));

    printf("%s \n", prt_str);

    // default fds 
    // 0 : system input  
    // 1 : system output 
    // 2 : system signal 
    // 3 : server socket 
    printf("fds_max =[%d]\n", fds_max);

    int fds, result;
    int clnt_sock, clnt_len;
    struct sockaddr_in clnt_addr;
    while(1)
    {
        tempfds = readfds;
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;

        //printbin(prt_str, &readfds, sizeof(readfds));
        result = select(fds_max+1, &tempfds, 0, 0,&timeout);

        if( result <= -1 )
        {
            error_handling("select() error");
        }
        else if( result == 0 )
        {
            printf("Timeout occured !!! : [%d] \n", fds_max);
            continue;
        }

        //After select the fds is reset to the changed fds that's why tempfds = readfds in the loop
        //After select the timeout value is set to the current waited time that's why timeout.tv_sec = 5 in the loop 

        // 0,1,2 system fds skip
        for( fds = 3; fds < fds_max+1; fds++)
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
                    rd.chkfds = readfds;
                    printf("add client fd set : %02X %02X %02X %02X \n", rd.x[0], rd.x[1], rd.x[2] , rd.x[3] );
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
                        printf("returned message to client [%d] : %s\n", fds, message);
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

int printbin(unsigned char *prt_str, void *buf, int len)
{
    unsigned char str[1024];
    unsigned char temp_c;
    unsigned char tmp_str[8+1];

    memset(str, 0x00, sizeof(str));
    memcpy(str, (unsigned char *)buf, len);

    printf("input : %02x %02X %d\n",str[0], str[1], len);

    for ( int i=0; i < len; i++)
    {
        memset(tmp_str, 0x00, sizeof(tmp_str));
        for ( int j=0; j < 8; j++)
        {
            temp_c = str[i] << j; 
            temp_c = temp_c >> 7;
            if ( temp_c != 0x01)
                tmp_str[j] = '0';
            else 
                tmp_str[j] = '1';            
        }
        memcpy(prt_str + i * 8, tmp_str, 8);
    }

    printf("%s\n", prt_str );

    return 0;
}
