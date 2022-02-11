/* multi_echo_server.c
   add incoming firewall IP & port ( CENTOS )
   0. sudo firewall-cmd --list-all-zones
   1. sudo firewall-cmd --zone=public --permanent --add-rich-rule='rule family="ipv4" source address="192.168.110.130/32" port protocol="tcp" port="9999" accept'  
   2. sudo cat /etc/firewalld/zones/public.xml
   3. sudo firewall-cmd --reload

   add incoming firewall IP & port ( UBUNTU )
   1. sudo ufw allow from 192.168.110.128 to any port 9999 ( proto tcp )
 
   network check
   1. sudo netstat -tulpn

   display TCP open port
   1. netstat -vatn | grep 9999
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>
#include <sys/wait.h>

#define BUFSIZE 100

void z_handler(int sig);
void error_handling(char *message);

int main( int argc, char **argv)
{
	int serv_sock;
	int clnt_sock;
	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	
	int clnt_addr_size, str_len;
	char message[BUFSIZE+1];
    static int fork_cnt = 0;
	
	if ( argc != 2){
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

    // signal handle start 
    pid_t pid;
    int state, i;

    struct sigaction act;
    act.sa_handler = z_handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    state = sigaction(SIGCHLD, &act, 0);
    if (state != 0)
    {
        puts("sigactin() error");
        exit(1);
    }
    // signal handle end 
	
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
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
	
    printf("listen() : server is ready for client request\n");

    while( 1 )
    {
        printf("server accept() waiting ... \n");
        clnt_addr_size = sizeof(clnt_addr);
        clnt_sock = accept(serv_sock, (struct sockaddr*) &clnt_addr, &clnt_addr_size);
        if( clnt_sock == -1)
            continue;

        printf("accept() : request from client ip[%s]/port[%d] is received / created client socket[%d] \n\n", 
                        inet_ntoa(clnt_addr.sin_addr), atoi(argv[1]), clnt_sock);
        
        pid = fork();

        if ( pid == -1 )
        {
            //fork error
            error_handling("fork() error");
            close(clnt_sock);
            continue;
        }

        printf("client process created : fork() count[%d]\n", ++fork_cnt);
        
        if ( pid > 0 )
        {
            puts("Parent process connected client");
            close(clnt_sock);
            continue;
        }
        else if ( pid == 0 )
        {
            close(serv_sock);
            memset(message, 0x00, str_len);
            // child process communicate to client
            while( ( str_len=read(clnt_sock, message, BUFSIZE)) != 0)
            {
                write(clnt_sock, message, str_len);
                write(1, message, str_len);
                memset(message, 0x00, str_len);
            }
            puts("client socket closed");
            close(clnt_sock);
            printf("client process exit : fork() count[%d]\n", --fork_cnt);
            exit(0);            
        }
        else
        {
            error_handling("fork() pid error");
            close(clnt_sock);
            continue;
        }
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

void z_handler(int sig)
{
    pid_t pid;
    int rtn;

    while ( pid = waitpid(-1, &rtn, WNOHANG) > 0)
    {
        printf("dropped zombie pid : %d\n", pid);
        printf("returned value     : %d \n\n", WEXITSTATUS(rtn));
    }
}