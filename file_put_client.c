/* file_put_client.c
   execution method
   file_put_client <IP> <port> <ID> <file_path> <file_name> 
   ex. file_put_client 127.0.0.1 9999 LGD001 /home/shlee/cpp/tcpip NEWS.txt
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
#define LEN_FILE_NAME 100

void error_handling(char *message);

int main( int argc, char **argv)
{
	int clnt_sock;
	struct sockaddr_in serv_addr;
	char buf[FILE_BUFSIZE+1];
	char file_name[LEN_FILE_NAME+1];
	int read_len;
	int id_len;
	
	if(argc != 6)
	{
		printf("Usage : %s <IP> <Port> <ID> <file_path> <File_name>\n", argv[0]);
		exit(1);
	}
	if(strlen(argv[3]) != 6)
	{
       printf("Send ID[%s] Incorrect\n", argv[3]);
	   exit(1);
	}    
    
	memset(file_name, 0, sizeof(file_name));
	memcpy(file_name, argv[4], strlen(argv[4]));
	memcpy(file_name + strlen(argv[4]), "/", 1);	
	memcpy(file_name + strlen(argv[4])+1, argv[5],strlen(argv[5]));

	printf("File to be sent [%s]\n", file_name);

    int fd = open(file_name, O_RDONLY);
    if ( fd == -1 )
        error_handling(" open() error !!!");

	clnt_sock = socket(PF_INET,SOCK_STREAM, 0);
	if(clnt_sock == -1)
		error_handling("socket() error");
    
    printf("socket() : socket created \n");

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port        = htons(atoi(argv[2]));
	
	if( connect(clnt_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1 )
		error_handling("connect() error");

	printf("connect() : request IP [%s] Port[%s] ID[%s] Path[%s] File[%s]\n", 
	                            argv[1], argv[2], argv[3], argv[4], argv[5]);
	
	memset(buf, 0x00, sizeof(buf));
	id_len = strlen(argv[3]);

	memcpy(buf         , argv[3], id_len);
	memcpy(buf+id_len  , "/"    , 1);
	memcpy(buf+id_len+1, argv[5], strlen(argv[5]));	
	printf("Send Request [%s]\n", buf);
	write(clnt_sock, buf, strlen(buf));
	sleep(1);
	memset(buf,0x00, sizeof(buf));

	int wait_timeout_cnt = 0;
	while( (read_len = read(clnt_sock, buf, sizeof(buf)) ) == 0)
	{
        printf("Wait for Server response !!!\n");
		sleep(1);
		wait_timeout_cnt++;

		if ( wait_timeout_cnt > 10 )
		    error_handling("Shinhan File Server is not responding now\n");
	}

	if ( memcmp(buf,"Ready for receiving OK",22) != 0)
	{
		printf("Received msg[%s]\n", buf);
		error_handling("Shinhan File Server is not ready\n");
	}
	printf("File sending Start\n");
	//
    while( (read_len = read(fd, buf, FILE_BUFSIZE)) != 0)
    {
		printf("Sending data ...\n");
        write(clnt_sock, buf, read_len);
		memset(buf, 0x00, sizeof(buf));
    }

	sleep(2);

 	memset(buf, 0x00, sizeof(buf));
	
	id_len = strlen(argv[3]);
    memcpy(buf         , "EOF-" , 4);
	memcpy(buf+4       , argv[3], id_len);
	memcpy(buf+id_len+4, "/"    , 1);
	memcpy(buf+id_len+5, argv[5], strlen(argv[5]));
	write(clnt_sock, buf, strlen(buf));	   

	printf("File sending Success\n");

	close(fd);
	close(clnt_sock);

	return 0;
}

void error_handling(char *message)
{
	fputs(message,stderr); 
	fputc('\n', stderr);
	exit(1);
}
