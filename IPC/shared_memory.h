#ifndef __SHARE_MEMORY_H__
#define __SHARE_MEMORY_H__

#define SHM_INFO_COUNT 30

typedef struct _shm_info
{
	unsigned int int_id;
	unsigned char str_msg[100];
} SHM_INFOS;
#endif//__SHARE_MEMORY_H__
