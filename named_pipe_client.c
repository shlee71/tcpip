/*
 * named pipe example between multi processes
 * client process
 */
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MSG_SIZE 80
#define PIPENAME_CTOS "./NAMED_PIPE_FILE_CTOS"
#define PIPENAME_STOC "./NAMED_PIPE_FILE_STOC"

int main(void) {
  char msg[MSG_SIZE];
  int fd1, fd2;
  int nread, i;

  /* named pipe open, for Write only */
  if ((fd1 = open(PIPENAME_CTOS, O_WRONLY)) < 0) {
      printf("fail to open named pipe[%s]\n", PIPENAME_CTOS);
      return 0;
  }
  for (i = 0; i < 3; i++) 
  {
      snprintf(msg, sizeof(msg), "Send Message[%i]", i);
      if ((nread = write(fd1, msg, sizeof(msg))) < 0 ) 
      {
          printf("fail to call write()\n");
      }
      printf("client : send message[%s] to pipe[%s]\n", msg, PIPENAME_CTOS);
  }

  printf("Client sleep for receiving message !!!\n");
  sleep(3);
  if ((fd2 = open(PIPENAME_STOC, O_WRONLY)) < 0) {
      printf("fail to open named pipe[%s]\n", PIPENAME_STOC);
      return 0;
  }
  // receive message from named pipe
  memset(msg, 0x00, sizeof(msg));
  nread = read(fd2, msg, sizeof(msg));
  printf("client : recv [%d] message[%s] from pipe[%s]\n", nread, msg, PIPENAME_STOC);
  return 0;
}
