#include "myapue.h"
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/uio.h>

int main(void)
{
    struct iovec iov[3];
    ssize_t nr;
    int fd, i;
    char foo[3][7];

    /* set up iovec structures */
    if ( (fd = open("temp.txt", O_RDONLY)) < 0 )
        err_sys("open error");
    for (i = 0; i < 3; i++) {
        iov[i].iov_base = foo[i];
        iov[i].iov_len = sizeof(foo[i]);
        printf("%zd\n", sizeof(foo[i]));
    }
    /* read into the structures with a single call */
    nr = readv(fd, iov, 3);
    if (nr < 0)
        err_sys("readv error");
    printf("readv %zd bytes\n", nr);
    for (i = 0; i < 3; i++)
        printf("%d: %s", i, (char *)iov[i].iov_base);
    if (close(fd) < 0)
        err_sys("close");

    exit(0);
}
