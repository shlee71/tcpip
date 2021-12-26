/* set_sockbuf.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

void error_handling(char *message);

int main( int argc, char **argv)
{
    int sock;

    int snd_buf = 0, rcv_buf = 0;

    int state;
    socklen_t sock_len;

    sock = socket(PF_INET, SOCK_STREAM, 0);

    state = getsockopt(sock, SOL_SOCKET, SO_SNDBUF, &snd_buf, &sock_len);
    if(state)
        error_handling("getsockopt() error");
    
    printf("Default send buf is %d \n", snd_buf);

    state = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, &rcv_buf, &sock_len);
    if(state)
        error_handling("getsockopt() error");
    
    printf("Default recv buf is %ld \n", rcv_buf);

    snd_buf = 1000;
    rcv_buf = 2000;

    state = setsockopt(sock, SOL_SOCKET, SO_SNDBUF, &snd_buf, sizeof(snd_buf));
    if(state)
        error_handling("setsockopt() error");
    
    state = setsockopt(sock, SOL_SOCKET, SO_RCVBUF, &rcv_buf, sizeof(rcv_buf));
    if(state)
        error_handling("setsockopt() error");

    state = getsockopt(sock, SOL_SOCKET, SO_SNDBUF, &snd_buf, &sock_len);
    if(state)
        error_handling("getsockopt() error");
    
    printf("Modified send buf is %d \n", snd_buf);

    state = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, &rcv_buf, &sock_len);
    if(state)
        error_handling("getsockopt() error");
    
    printf("Modified recv buf is %ld \n", rcv_buf);

    return 0;
    
}
void error_handling(char *message)
{
	fputs(message,stderr); 
	fputc('\n', stderr);
	exit(1);
}