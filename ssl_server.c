/* ssl_server.c
   add incoming firewall IP & port ( CENTOS )
   1. sudo firewall-cmd --zone=public --permanent --add-rich-rule='rule family="ipv4" source address="192.168.110.130/32" port protocol="tcp" port="8081" accept'  
   2. sudo cat /etc/firewalld/zones/public.xml
   3. sudo firewall-cmd --reload

   add incoming firewall IP & port ( UBUNTU )
   1. sudo ufw allow from 192.168.110.128 to any port 8081
 
   network check
   1. sudo netstat -tulp

   open file descriptor
   1. lsof -p <pid>  : ex. lsof -p 28290

   Pre-arrangement : make privatekey.pem , cert.pem
     openssl req -x509 -nodes -days 365 -newkey rsa:2048 -keyout myprivatekey.pem -out mycert.pem

   compile method
     gcc -Wall -o ssl_server ssl_server.c  -L/usr/lib -lssl -lcrypto

   execution
     ./ssl_server 8081

 */
#include <errno.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <resolv.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#define FAIL    -1
//#define OPENSSL_NO_TLS1_2_METHOD
 
// Create the SSL socket and intialize the socket address structure
int OpenListener(int port)
{
    int serv_sock;
    struct sockaddr_in addr;
 
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(serv_sock, (struct sockaddr*)&addr, sizeof(addr)) != 0 )
    {
        perror("can't bind port");
        abort();
    }
    if ( listen(serv_sock, 10) != 0 )
    {
        perror("Can't configure listening port");
        abort();
    }

    return serv_sock;
}
 
//int isRoot()
//{
//    if (getuid() != 0)
//    {
//        return 0;
//    }
//    else
//    {
//        return 1;
//    }
// 
//}
SSL_CTX* InitServerCTX(void)
{
    SSL_METHOD *method;
    SSL_CTX *ctx;

    /* load & register all cryptos, etc. */
    OpenSSL_add_all_algorithms();
    /* load all error messages */
    SSL_load_error_strings();
    /* create new server-method instance */
    method = (SSL_METHOD *)TLS_server_method();
    /* create new context from method */
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
 
void LoadCertificates(SSL_CTX* ctx, char* CertFile, char* KeyFile)
{
    /* set the local certificate from CertFile */
    if ( SSL_CTX_use_certificate_file(ctx, CertFile, SSL_FILETYPE_PEM) <= 0 )
    {
        ERR_print_errors_fp(stderr);
        abort();
    }
    /* set the private key from KeyFile (may be the same as CertFile) */
    if ( SSL_CTX_use_PrivateKey_file(ctx, KeyFile, SSL_FILETYPE_PEM) <= 0 )
    {
        ERR_print_errors_fp(stderr);
        abort();
    }
    /* verify private key */
    if ( !SSL_CTX_check_private_key(ctx) )
    {
        fprintf(stderr, "Private key does not match the public certificate\n");
        abort();
    }
}
 
void ShowCerts(SSL* ssl)
{
    X509 *cert;
    char *line;
 
    /* Get certificates (if available) */
    cert = SSL_get_peer_certificate(ssl); 
    if ( cert != NULL )
    {
        printf("Server certificates:\n");
        line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
        printf("Subject: %s\n", line);
        free(line);
        line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
        printf("Issuer: %s\n", line);
        free(line);
        X509_free(cert);
    }
    else
    {
        printf("No certificates.\n");
    }
}
/* Serve the connection -- threadable */
void Servlet(SSL* ssl) 
{
    char buf[1024] = {0};
   
    int serv_sock, bytes;
    const char* ServerResponse="Welcome to ssl_server, Mr. ssl_client.\nNice to meet you !!!";
    const char *cpValidMessage="UserName:shlee, Password:123456";
 /*   
 const char* ServerResponse="<\Body>\
<Name>aticleworld.com</Name>\
<year>1.5</year>\
<BlogType>Embedede and c\c++<\BlogType>\
<Author>amlendra<Author>\
<\Body>";

const char *cpValidMessage="<Body>\
<UserName>shlee<UserName>\
<Password>1234<Password>\
<\Body>"; 
*/ 
    /* do SSL-protocol accept */
    if ( SSL_accept(ssl) == FAIL )
    {
        ERR_print_errors_fp(stderr);
    }
    else
    {
        /* get any certificates */
        ShowCerts(ssl);
        /* get request */
        bytes = SSL_read(ssl, buf, sizeof(buf));
        buf[bytes] = '\0';
 
        printf("Message from client: \"%s\"\n", buf);
 
        if ( bytes > 0 )
        {
            if(strcmp(cpValidMessage,buf) == 0)
            {
                /* send reply */
                SSL_write(ssl, ServerResponse, strlen(ServerResponse)); 
            }
            else
            {
                /* send reply */
                SSL_write(ssl, "Invalid Message", strlen("Invalid Message"));
            }
        }
        else
        {
            ERR_print_errors_fp(stderr);
        }
            
    }
    /* get socket connection */
    serv_sock = SSL_get_fd(ssl);
    /* release SSL state */
    SSL_free(ssl);
    /* close connection */
    close(serv_sock);
}
 
int main(int argc, char **argv)
{
    SSL_CTX *ctx;
    int serv_sock;
    char *port;

    /*
    //Commented Only root user have the permsion to run the server
    if(!isRoot())
    {
        printf("This program must be run as root/sudo user!!");
        exit(0);
    }
    */
    if ( argc != 2 )
    {
        printf("Usage: %s <portnum>\n", argv[0]);
        exit(0);
    }
 
    // Initialize the SSL library
    SSL_library_init();
 
    port = argv[1];
    /* initialize SSL */
    ctx = InitServerCTX();

    /* load certs : made by openssl Server*/
    LoadCertificates(ctx, "mycert.pem", "myprivatekey.pem"); 
    /* create server socket */
    serv_sock = OpenListener(atoi(port));

    while (1)
    {
        struct sockaddr_in addr;
        socklen_t len = sizeof(addr);
        SSL *ssl;

        /* accept connection */
        int client = accept(serv_sock, (struct sockaddr*)&addr, &len);
        printf("Connection: %s:%d\n",inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
        /* get new SSL state with context */
        ssl = SSL_new(ctx);
        /* set connection socket to SSL state */
        SSL_set_fd(ssl, client);
        /* service connection */
        Servlet(ssl);
    }
    close(serv_sock);          /* close server socket */
    SSL_CTX_free(ctx);         /* release context */
}
