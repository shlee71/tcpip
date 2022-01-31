#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <string.h> 
#include <unistd.h> 
#include "shared_memory.h"

#define KEY_ID  1234567

int main()
{
	int shmid;
	int i;
	SHM_INFOS *shm_info = NULL;
	void *shared_memory = (void *)0;

	// request system kernel to create shared memory
	shmid = shmget((key_t)KEY_ID, sizeof(SHM_INFOS)*SHM_INFO_COUNT, 0666|IPC_CREAT);

	if (shmid == -1)
	{
		perror("shmget failed : ");
		exit(0);
	}

	printf("\n### Shared memory after shmget() ###\n");
	system("ipcs -m");

	// connect NULL pointer kernel's allocated shared memory to process memory 
    shared_memory = shmat(shmid, (void *)0, 0);
	if (shared_memory == (void *)-1)
	{
		perror("shmat failed : ");
		exit(0);
	}

	printf("\n### Shared memory after shmat() ###\n");
	system("ipcs -m");

    // connect proess variable to shared memory
	shm_info = (SHM_INFOS *)shared_memory;

	while(1)
	{
		for(i=0 ;i < SHM_INFO_COUNT; i++)
		{
			snprintf(shm_info[i].str_msg,sizeof(shm_info[i].str_msg),"Count Down [%d]", SHM_INFO_COUNT - i);
			shm_info[i].int_id = i;
		}

        break;
	}

	printf("\n### Shared memory after write ###\n");
	system("ipcs -m");

	return 0;
}
