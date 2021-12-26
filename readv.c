/*
    readv.c
    struct iovec {
        ptr_t iov_base;
        size_t iov_len;
    }
*/
#include <stdio.h>
#include <sys/uio.h>

int main(void)
{
	struct iovec vec[2];
	char MSG1[10] = {0,};
	char MSG2[10] = {0,};

    vec[0].iov_base = MSG1;
    vec[0].iov_len = 9;

    vec[1].iov_base = MSG2;
    vec[1].iov_len = 9;
    
    int str_len=readv(0,vec,2);

	printf("Total byte [%d]\n", str_len);
	printf("1st message : [%s]\n", MSG1);
    printf("2nd message : [%s]\n", MSG2);
	
    return 0;
}
