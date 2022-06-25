/* multi_echo_server.c
   add incoming firewall IP & port ( CENTOS )
   0. sudo firewall-cmd --list-all-zones
   1. sudo firewall-cmd --zone=public --permanent --add-rich-rule='rule family="ipv4" source address="192.168.110.130/32" port protocol="tcp" port="9999" accept'  
   2. sudo cat /etc/firewalld/zones/public.xml
   3. sudo firewall-cmd --reload

   add incoming firewall IP & port ( UBUNTU )
   1. sudo ufw allow from 192.168.110.128 to any port 9999 ( proto tcp )
 
   network check
   1. sudo netstat -antulpn

   display TCP open port
   1. netstat -vatn | grep 9999

   compile
   gcc -D_REENTRANT multithread_echo_server.c -o multithread_echo_server -lpthread
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>
#include <sys/wait.h>
#include <pthread.h>

#define BUFSIZE 100
#define MAX_THREAD_LIMIT 20

int child_process_cnt = 0;
int thread_index = 0;

typedef struct {
    bool alive;
    int clnt_sock;
    int t_index;
    pthread_t t_id;
} thread_info;

thread_info serv_thread[MAX_THREAD_LIMIT];
thread_info clnt_thread; 

void *echo_client_thread(void *arg);
pthread_mutex_t mutx;
pthread_t thread_id[MAX_THREAD_LIMIT];
void *t_return;

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
    
	if ( argc != 2){
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

    // signal handle start 
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

    if(pthread_mutex_init(&mutx, NULL))
        error_handling("pthread_mutex_init() error");
	
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
        
        if ( child_process_cnt < MAX_THREAD_LIMIT)
        {
            printf("server accept() waiting ... \n");
            clnt_addr_size = sizeof(clnt_addr);
            clnt_sock = accept(serv_sock, (struct sockaddr*) &clnt_addr, &clnt_addr_size);
            if( clnt_sock == -1)
            {
                printf("accept() Error !!! \n");
                continue;
            }
            printf("accept() : request from client ip[%s]/port[%d] is received / created client socket[%d] \n\n", 
                            inet_ntoa(clnt_addr.sin_addr), atoi(argv[1]), clnt_sock);

            printf("New client connected, client ip : %s \n", inet_ntoa(clnt_addr.sin_addr));

            pthread_mutex_lock(&mutx);
            child_process_cnt++;
            for ( i =0; i< MAX_THREAD_LIMIT; i++)
            {
                if (serv_thread[i].alive == false)
                {
                    thread_index = i;
                    break;
                }
            }
            clnt_thread.alive = true;
            clnt_thread.clnt_sock = clnt_sock;
            clnt_thread.t_index   = thread_index;
            serv_thread[thread_index].alive = true;  
            serv_thread[thread_index].clnt_sock = clnt_sock;            
            serv_thread[thread_index].t_index = thread_index;
            pthread_mutex_unlock(&mutx);

            //state = pthread_create(&thread_id[thread_index], NULL, echo_client_thread, (void *)clnt_sock);
            state = pthread_create(clnt_thread.t_id, NULL, echo_client_thread, (void *)&clnt_thread);
            if (state != 0)
            {
                printf("pthread_create() error\n");
                continue;
            }

            printf("child thread counter increased [%d] / index[%d]\n", child_process_cnt, thread_index);

        }

        puts("Parent process connected client");
    }

    printf("End of Loop & arrange the ending process\n");
    for ( i=0; i< MAX_THREAD_LIMIT; i++)
    {
        if (thread_id[i] > 0)
        {
            state = pthread_join(thread_id[i], &t_return);
            if( state != 0 )
            {
                 printf("pthread_join() error\n");
            }
        }
    }

    pthread_exit(NULL);

	close(serv_sock);
	return 0;
}
void *echo_client_thread(void *arg)
{
    thread_info clnt_thread = (thread_info *)arg;
    int clnt_sock = clnt_thread.clnt_sock;
    int str_len = 0;

    char message[BUFSIZE];
    int i = 0;

    printf("echo_client_thread start [%d]\n", clnt_sock);
    while(( str_len = read(clnt_sock, message, sizeof(message))) != 0 )
    {
        if ( str_len > 0 )
        {
            write(clnt_sock, message, str_len);
            printf("echo message : [%d]/%s\n", str_len, message);
        }
        sleep(1);
        memset(message, 0x00, sizeof(message));
    }
    pthread_mutex_lock(&mutx);
    child_process_cnt--;
    pthread_mutex_unlock(&mutx);

    printf("child thread counter decreased [%d]\n",child_process_cnt);

    close(clnt_sock);
}
void z_handler(int sig)
{
    int i;

    for ( i=0; i< MAX_THREAD_LIMIT; i++)
    {
        if (thread_id[i] > 0)
            pthread_join(thread_id[i], &t_return);
    }

    pthread_exit(NULL);
}
void error_handling(char *message)
{
	fputs(message,stderr); 
	fputc('\n', stderr);
	exit(1);
}
