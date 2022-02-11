/* gethostbyname.c
   struct hostent{
       char *h_name;       // Official name
       char **h_aliases;   // Alias list
       int h_addrtype;     // Host address type : INET vs INET6
       int h_length;       // Length of Address
       char **h_addr_list; // List of Address
   }

   EXECUTION Method : gethostbyname shlee.centos8.com
   RESULT :   
        Officially name : shlee.centos8.com.localdomain

        Aliases--------
        Address Type : AF_INET
        IP Address------
        192.168.110.128

 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

void error_handling(char *message);

int main( int argc, char **argv)
{
    int i;
    struct hostent *host;
	
	if ( argc != 2){
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

    host = gethostbyname(argv[1]);
    if(!host)
        error_handling("gethost..error");
    
    printf("Officially name : %s \n\n", host->h_name);

    puts("Aliases--------");

    for( i =0; host->h_aliases[i]; i++)
        puts( host->h_aliases[i] );
    
    printf("Address Type : %s \n", host->h_addrtype == AF_INET? "AF_INET": "AF_INET6");
    puts("IP Address------");

    for( i =0; host->h_addr_list[i]; i++)
        puts( inet_ntoa( *(struct in_addr*)host->h_addr_list[i] ));    
    
    return 0;
}
void error_handling(char *message)
{
	fputs(message,stderr); 
	fputc('\n', stderr);
	exit(1);
}
