/* oob_recv.c
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>

#define BUFSIZE 100

void urg_handler(int sig);
void error_handling(char *message);

int serv_sock;
int clnt_sock;

int main( int argc, char **argv)
{

	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	
	int clnt_addr_size, str_len;
	char message[BUFSIZE+1];
	
	if ( argc != 2){
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if(serv_sock == -1)
		error_handling("socket() error");
	
    printf("server socket() created \n");

    // signal handle start 
    pid_t pid;
    int state, i;

    struct sigaction act;
    act.sa_handler = urg_handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    // signal handle end 

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
    
    clnt_addr_size = sizeof(clnt_addr);
    clnt_sock =accept(serv_sock, (struct sockaddr*) &clnt_addr, &clnt_addr_size);

    // owner change clnt_sock caller pid
    fcntl(clnt_sock, F_SETOWN, getpid()); 

    state = sigaction(SIGURG, &act, 0);
    if (state != 0)
    {
        puts("sigactin() error");
        exit(1);
    }
    sleep(1);
    
    while( str_len = recv(clnt_sock, message, sizeof(message), 0) != 0 )
    {
        if ( str_len == -1)
            continue;
        message[BUFSIZE] = 0;
        puts(message);
        memset(message, 0x00, sizeof(message));
    }

    close(clnt_sock);
	close(serv_sock);
	return 0;
}

void urg_handler(int sig)
{
    int str_len;
    char buf[BUFSIZE];

    str_len = recv(clnt_sock, buf, sizeof(buf) -1, MSG_OOB );
    buf[str_len] = 0;
    printf("Urgent message received : %s \n", buf);
}

void error_handling(char *message)
{
	fputs(message,stderr); 
	fputc('\n', stderr);
	exit(1);
}
