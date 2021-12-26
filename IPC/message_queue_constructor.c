/*
 * Queue constructor & send message
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/stat.h> 
#include <arpa/inet.h>
#include <sys/socket.h>

#define KEY_TYPE 123456

struct msgbuf1
{
  // Fixed definiton for message queue id
  long msgtype;      
  // Variable structure for message flexibility
  char mtext[256];  
  char myname[16];
  int  seq;
};

struct msgbuf2
{
  // Fixed definiton for message queue id
  long msgtype;      
  // Variable structure for message flexibility
  char ip[128];  
  char host[16];
  int  seq;
};


int main(void)
{
  key_t key_id;
  int i;
  struct msgbuf1 msgbuf1;
  struct msgbuf2 msgbuf2;

  // 1. Message Queue Allocation
  key_id = msgget((key_t)KEY_TYPE, IPC_CREAT|0666);
  if (key_id == -1)
  {
    perror("msgget error : ");
    return 0;
  }
  // 2. Print allocated Queue ID 
  printf("Allocated Key ID [%d]\n", key_id);

  msgbuf1.seq = 0;
  i = 0;
  while (1) 
  {
      memset(&msgbuf1, 0, sizeof(struct msgbuf1));
      memset(&msgbuf2, 0, sizeof(struct msgbuf2));

      // Even number msg type 2
      // Odd  number msg type 1
      if (i % 2 == 1) 
      {
          msgbuf1.msgtype = 1;
          snprintf(msgbuf1.mtext, sizeof(msgbuf1.mtext), "hello odd [%d]",i);
          snprintf(msgbuf1.myname, sizeof(msgbuf1.myname), "name [%d]",i);
          msgbuf1.seq = i;

          // 3. Send message
          if (msgsnd( key_id, (void *)&msgbuf1, sizeof(struct msgbuf1), IPC_NOWAIT) == -1)
          {
              perror("msgsnd error : ");
              return 0;
          } 
      } 
      else 
      {
          msgbuf2.msgtype = 2;
          snprintf(msgbuf2.ip, sizeof(msgbuf2.ip), "ip[10.0.0.%d]",i);
          snprintf(msgbuf2.host, sizeof(msgbuf2.host), "host_%d",i);
          msgbuf2.seq = i;
          // 3. Send message
          if (msgsnd( key_id, (void *)&msgbuf2, sizeof(struct msgbuf2), IPC_NOWAIT) == -1)
          {
              perror("msgsnd error : ");
              return 0;
          }
      }

      printf("send %d\n", i);
      i++;
      sleep(1);
  }
  return 0;
}