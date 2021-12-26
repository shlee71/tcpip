/* 
 * select_test.c
 * input message ?
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>

#define BUFSIZE 30

int main(int argc, char **argv)
{
    fd_set reads, temps;
    int result;

    char message[BUFSIZE];
    int str_len;
    struct timeval timeout;

    FD_ZERO(&reads);
    FD_SET(0, &reads); /* standard input read set */

    /* timeout.tv_sec = 5;
       timeout.tv_usec = 0;
    */
   while(1)
   {
       temps = reads;
       timeout.tv_sec = 5;
       timeout.tv_usec = 0;

       result = select(1, &temps, 0,0, &timeout);
       if ( result == -1){
           puts("select error occured !!!");
       }
       else if ( result == 0) { 
           puts("timeout occured select()");
       }
       else {
           // if 0th File descripter is set then read 0th message and fputs 
           if(FD_ISSET(0, &temps)) {
               str_len = read(0, message, BUFSIZE);
               message[str_len] = 0;
               fputs(message, stdout);
           }
       }
   }

   return 0;
}