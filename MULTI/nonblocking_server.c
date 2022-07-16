#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

#define BUF_SIZE 1024

void other_routine();

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Usage : %s [port]\n", argv[0]);
        exit(1);
    }

    int server_sock, client_sock;
    char buf[BUF_SIZE];

    if((server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0 )
    {
        printf("socket create error\n");
        exit(1);
    }

    int on = 1;
    if(setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
    {
        printf("socket option set error\n");
        exit(1);
    }

    struct sockaddr_in server_addr, client_addr;
    int client_addr_size = sizeof(client_addr);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[1]));

    if(bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0 )
    {
        printf("bind error\n");
        exit(1);
    }

    if(listen(server_sock, 5) < 0)
    {
        printf("listen error\n");
        exit(1);
    }

    printf("----- accept wait\n");

    client_sock = accept(server_sock, (struct sockaddr *)&client_addr, (socklen_t *)&client_addr_size);

    if(client_sock < 0)
    {
        printf("accept error\n");
        exit(1);
    }

    printf("----- accept client\n");

    int flag = fcntl(client_sock, F_GETFL, 0);
    fcntl(client_sock, F_SETFL, flag | O_NONBLOCK);

    int read_rtn;

    while(1)
    {
        sleep(1);

        printf("----- read wait\n");

        memset(buf, 0x00, sizeof(buf));
        read_rtn = read(client_sock, buf, sizeof(buf));

        if (read_rtn < 0)
        {
            // EAGAIN definds try again #11 
            if(errno == EAGAIN)
            {
                printf("----- read EAGAIN\n");
            }
            else
            {
                printf("----- read error\n");
                printf("----- Socket close\n");
                close(client_sock);
                break;
            }

        }
        else if(read_rtn == 0)
        {
            printf("----- Socket close\n");
            close(client_sock);
            break;
        }
        else
        {
            printf("----- read : %s\n", buf);
        }

        other_routine();
    }

    return 0;

}

void other_routine()
{
    printf("----- Other rutine processing...\n");
}
