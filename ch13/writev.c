/*
    write.c
    struct iovec {
        ptr_t iov_base;
        size_t iov_len;
    }
*/
#include <stdio.h>
#include <sys/uio.h>
#include <string.h>

int main(void)
{
	struct iovec vec[2];
	char MSG1[] = "Computer ";
	char MSG2[] = "Communications";

    vec[0].iov_base = MSG1;
    vec[0].iov_len  = strlen(MSG1);

    vec[1].iov_base = MSG2;
    vec[1].iov_len = strlen(MSG2);
    
    int str_len=writev(1,vec,2);
	printf("Total byte [%d]\n", str_len);
	
    return 0;
}
