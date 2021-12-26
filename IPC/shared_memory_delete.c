#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <string.h> 
#include <unistd.h> 
#include "shared_memory.h"

#define KEY_ID  1234

int main()
{
	int shmid;

	void *shared_memory = (void *)0;

    // request system kernel to create shared memory
	shmid = shmget((key_t)KEY_ID, sizeof(SHM_INFOS)*SHM_INFO_COUNT, 0666|IPC_CREAT);

	if (shmid == -1)
	{
		perror("shmget failed : ");
		exit(0);
	}

    // remove shared memory
    if ( -1 == shmctl( shmid, IPC_RMID, 0))
	{
		printf( "Shared memor remove failed\n");
		return -1;
	}
	else
	{
		printf( "Shared memory delete success\n");
	}

    return 0;
}