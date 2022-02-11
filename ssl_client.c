/* ssl_client.c
   add incoming firewall IP & port ( CENTOS )
   1. sudo firewall-cmd --zone=public --permanent --add-rich-rule='rule family="ipv4" source address="192.168.110.130/32" port protocol="tcp" port="8081" accept'  
   2. sudo cat /etc/firewalld/zones/public.xml
   3. sudo firewall-cmd --reload

   add incoming firewall IP & port ( UBUNTU )
   1. sudo ufw allow from 192.168.110.128 to any port 8081
 
   compile method
     gcc -Wall -o ssl_client ssl_client.c  -L/usr/lib -lssl -lcrypto

   execution
     ./ssl_client 192.168.110.128 8081

 */
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <resolv.h>
#include <netdb.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
 
#define FAIL    -1
#define OPENSSL_NO_TLS1_2_METHOD
 
int OpenConnection(const char *serv_addr, int port)
{
    int serv_sock;
    struct sockaddr_in addr; 
 /*
    struct hostent *host;
    if ( (host = gethostbyname(inet_addr)) == NULL )
    {
        perror(inet_addr);
        abort();
    }
*/
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    //addr.sin_addr.s_addr = *(long*)(host->h_addr);
    addr.sin_addr.s_addr = inet_addr(serv_addr);
    if ( connect(serv_sock, (struct sockaddr*)&addr, sizeof(addr)) != 0 )
    {
        close(serv_sock);
        perror(serv_addr);
        abort();
    }
    return serv_sock;
}
 
SSL_CTX* InitCTX(void)
{
    SSL_METHOD *method;
    SSL_CTX *ctx;
 
    /* Load cryptos, et.al. */
    OpenSSL_add_all_algorithms();
    /* Bring in and register error messages */
    SSL_load_error_strings();
    /* Create new client-method instance */
    method = (SSL_METHOD *)TLS_client_method();
    /* Create new context */
    ctx = SSL_CTX_new(method);
    if ( ctx == NULL )
    {
        ERR_print_errors_fp(stderr);
        abort();
    }

    //SSL_CTX_set_min_proto_version(ctx, TLS1_2_VERSION);
    //SSL_CTX_set_max_proto_version(ctx, TLS1_2_VERSION);

    return ctx;
}

void ShowCerts(SSL* ssl)
{
    X509 *cert;
    char *line;
 
    cert = SSL_get_peer_certificate(ssl); /* get the server's certificate */
    if ( cert != NULL )
    {
        printf("Server certificates:\n");
        line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
        printf("Subject: %s\n", line);
        /* free the malloc'ed string */
        free(line);
        line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
        printf("Issuer: %s\n", line);
        /* free the malloc'ed string */
        free(line);
        /* free the malloc'ed certificate copy */
        X509_free(cert);
    }
    else
    {
        printf("Info: No client certificates configured.\n");
    }
}
int main(int argc, char **argv)
{
    SSL_CTX *ctx;
    int server;
    SSL *ssl;
    char buf[1024];
    char acClientRequest[1024] ={0};
    int bytes;
    char *addr, *port;
 
    if ( argc != 3 )
    {
        printf("usage: %s <hostname> <port>\n", argv[0]);
        exit(0);
    }
 
    SSL_library_init();
    addr=argv[1];
    port=argv[2];
    ctx = InitCTX();

    /* socket open & connect */
    server = OpenConnection(addr, atoi(port));
    /* create new SSL connection state */
    ssl = SSL_new(ctx);
    /* attach the socket descriptor */      
    SSL_set_fd(ssl, server);
    /* perform the connection */
    if ( SSL_connect(ssl) == FAIL )
    {
        ERR_print_errors_fp(stderr);
    }
    else
    {
        char acUsername[16] ={0};
        char acPassword[16] ={0};
        const char *cpRequestMessage="UserName:%s, Password:%s";
/*
        const char *cpRequestMessage = "<Body>\
<UserName>%s<UserName>\
<Password>%s<Password>\
<\Body>";
*/
        printf("Enter the User Name : ");
        scanf("%s",acUsername);

        printf("\nEnter the Password : ");
        scanf("%s",acPassword);

        sprintf(acClientRequest, cpRequestMessage, acUsername,acPassword);   
        /* construct reply */
        printf("\n\nConnected with %s encryption\n", SSL_get_cipher(ssl));
        /* get any certs */
        ShowCerts(ssl);
        /* encrypt & send message */
        SSL_write(ssl,acClientRequest, strlen(acClientRequest));
        /* get reply & decrypt */
        bytes = SSL_read(ssl, buf, sizeof(buf)); 
        buf[bytes] = 0;
        printf("\nReceived Message: \n%s\n", buf);
         /* release connection state */
        SSL_free(ssl);
    }
    /* close socket */
    close(server);
    /* release context */
    SSL_CTX_free(ctx);
    return 0;
}