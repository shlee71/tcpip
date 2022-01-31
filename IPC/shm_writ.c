#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 

int main(int argc, char *argv)
{
    // ftok to generate unique key
    key_t key = ftok("shmfile",65);
  
    // shmget returns an identifier in shmid
    int shmid = shmget(key,1024,0666|IPC_CREAT);
    system("ipcs -m");
  
    // shmat to attach to shared memory
    char *str = (char*) shmat(shmid,(void*)0,0);
    system("ipcs -m");
  
    printf("Write Data : ");
    fgets(str, sizeof(str), stdin);
 
    printf("Data written in memory: %s\n",str);
 
    //disconnect(ditach) the process from shared memory 
    shmdt(str);
    system("ipcs -m");
  
    return 0;
}