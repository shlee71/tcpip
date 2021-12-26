/*
 * Queue consumer & receive message
 * execution : message_queue_consumer 1 or 2 ...
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/stat.h> 
#include <ctype.h>

#define KEY_TYPE 123456

struct msgbuf1 {
    // Fixed declaration variable for message type
    long msgtype;
    // Variable structure area
    char mtext[256];
    char myname[16];
    int  seq;
};

struct msgbuf2 {
  // Fixed declaration variable for message type
  long msgtype;      
  // Variable structure area
  char ip[128];  
  char host[16];
  int  seq;
};

int msgtype;

int main(int argc, char **argv)
{
    key_t key_id;
    struct msgbuf1 mybuf1;
    struct msgbuf2 mybuf2;

    if ( argc != 2 )
    {
        printf("usage : %s number(1..2) \n", argv[0]);
        return 0;        
    }

    if ( isdigit(argv[1][0]) <= 0 )
    {
        printf("usage : %s number(1..2) \n", argv[0]);
        return 0;        
    }

    // check argument & decide the msg type   
    if ( atol(argv[1]) % 2 == 0 )
        msgtype = 2;
    else 
        msgtype = 1;

    key_id = msgget(KEY_TYPE, IPC_CREAT|0666);
    if (key_id < 0) 
    {
        perror("msgget error : ");
        return 0;        
    }

    // Print allocated Queue ID 
    printf("Allocated Key ID [%d]\n", key_id);

    while(1) 
    {
        if (msgtype == 2) 
        {
            if (msgrcv( key_id, (void *)&mybuf2, sizeof(struct msgbuf2), msgtype, 0) == -1)
            {
              perror("msgrcv error : ");
              return 0;
            }
            printf("%s %s %d\n",mybuf2.ip, mybuf2.host, mybuf2.seq);
        } 
        else 
        {
            if (msgrcv( key_id, (void *)&mybuf1, sizeof(struct msgbuf1), msgtype, 0) == -1)
            {
              perror("msgrcv error : ");
              return 0;
            }
            printf("%s %d\n",mybuf1.mtext, mybuf1.seq);
        }
    }
    return 0;
}