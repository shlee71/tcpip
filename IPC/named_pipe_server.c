/*
 * named pipe example between multi processes
 * server process
 */
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MSG_SIZE 80
// client(consumer) to server(constructor) PIPE 
#define PIPENAME_CTOS "./NAMED_PIPE_FILE_CTOS"
// server to client PIPE 
#define PIPENAME_STOC "./NAMED_PIPE_FILE_STOC"

int main(void) {
  char msg[MSG_SIZE];
  int fd1, fd2;
  int nread, rc;

  /* DELETE PIPE IF ALREADY EXIST */
  if (access(PIPENAME_CTOS,F_OK) == 0) {
      unlink(PIPENAME_CTOS);
  }

  /* named pipe create */
  if ((rc = mkfifo(PIPENAME_CTOS,0666)) < 0) {
      printf("fail to make named pipe\n");
      return 0;
  }

  /* named pipe open for read & write */
  if ((fd1 = open(PIPENAME_CTOS, O_RDWR)) < 0) {
      printf("fail to open named pipe[%s]\n", PIPENAME_CTOS);
      return 0;
  }

  //while (1) changed for exit loop & send data for test 
  printf("server ready for receiving data !!!\n\n");
  for ( int i = 0; i < 3; i++) 
  {
      if ((nread = read(fd1, msg, sizeof(msg))) < 0 ) 
      {
          printf("fail to call read()\n");
          break;
      }
      else 
      {        
          printf("server : recv message[%s] from named pipe [%s]\n", msg, PIPENAME_CTOS);
      }    
  }

  /* DELETE PIPE IF ALREADY EXIST */
  if (access(PIPENAME_STOC,F_OK) == 0) {
      unlink(PIPENAME_STOC);
  }

  /* named pipe create */
  if ((rc = mkfifo(PIPENAME_STOC,0666)) < 0) {
      printf("fail to make named pipe[%s]\n", PIPENAME_STOC);
      return 0;
  }

  /* named pipe open for write */
  if ((fd2 = open(PIPENAME_STOC, O_WRONLY)) < 0) {
      printf("fail to open named pipe[%s]\n", PIPENAME_STOC);
      return 0;
  }
  // send message to the named pipe
  memset(msg, 0x00, sizeof(msg));
  strncpy(msg, "Good bye client", sizeof(msg));
  printf("server : send message [%s] to client\n\n", msg);
  write(fd2, msg, sizeof(msg));

  sleep(1);
  /* */
  return 0;
}