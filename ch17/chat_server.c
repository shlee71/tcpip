/* chat_server.c
   add incoming firewall IP & port ( CENTOS )
   1. sudo firewall-cmd --zone=public --permanent --add-rich-rule='rule family="ipv4" source address="192.168.110.130/32" port protocol="tcp" port="9999" accept'  
   2. sudo cat /etc/firewalld/zones/public.xml
   3. sudo firewall-cmd --reload

   add incoming firewall IP & port ( UBUNTU )
   1. sudo ufw allow from 192.168.110.128 to any port 9999
 
   network check
   1, sudo netstat -tulpn

   compile
    gcc -D_REENTRANT chat_server.c -o chat_server -lpthread
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define BUFSIZE 100

void *clnt_connection(void *arg);
void send_message(char *message, int len);
void error_handling(char *message);

int clnt_number = 0;
int clnt_sock_array[10];
pthread_mutex_t mutx;

int main( int argc, char **argv)
{
	int serv_sock;
	int clnt_sock;
	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	
	int clnt_addr_size;
	char message[BUFSIZE];

    pthread_t thread;
	
	if ( argc != 2){
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

    if(pthread_mutex_init(&mutx, NULL))
        error_handling("pthread_mutex_init() error");
    
	serv_sock = socket(PF_INET,SOCK_STREAM, 0);
	if(serv_sock == -1)
		error_handling("socket() error");
	
    printf("socker() created \n");

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
            error_handling("accept() error");
        
        pthread_mutex_lock(&mutx);
        clnt_sock_array[clnt_number++]= clnt_sock;
        pthread_mutex_unlock(&mutx);

        pthread_create(&thread, NULL, clnt_connection, (void *)clnt_sock);
        printf("New client connected, client ip : %s \n", inet_ntoa(clnt_addr.sin_addr));
    }

	close(clnt_sock);
	close(serv_sock);
	return 0;
}

void * clnt_connection(void *arg)
{
    int clnt_sock = (int)arg;
    int str_len = 0;

    char message[BUFSIZE];
    int i;

    while(( str_len = read(clnt_sock, message, sizeof(message))) !=0)
        send_message(message, str_len);
    
    pthread_mutex_lock(&mutx);
    for(i =0; i<clnt_number ; i++)
    {
        if ( clnt_sock == clnt_sock_array[i])
        {
            for(; i< clnt_number -1; i++)
                clnt_sock_array[i]= clnt_sock_array[i+1];
            break;
        }
    }
    clnt_number--;
    pthread_mutex_unlock(&mutx);

    close(clnt_sock);
    return 0;
}

void send_message(char *message, int len)
{
    int i;
    pthread_mutex_lock(&mutx);
    for(i =0; i< clnt_number; i++)
        write(clnt_sock_array[i], message, len);
    pthread_mutex_unlock(&mutx);
}
void error_handling(char *message)
{
	fputs(message,stderr); 
	fputc('\n', stderr);
	exit(1);
}
