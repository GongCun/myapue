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

    char *buf[] = {"buf 0\n", "buf 1\n", "buf 2\n"};
    fd = open("temp.txt", O_RDWR|O_CREAT|O_TRUNC, FILE_MODE);
    if (fd < 0)
        err_sys("creat error");

    /* fill out three iovec structures */
    for (i = 0; i < 3; i++) {
        iov[i].iov_base = buf[i];
        iov[i].iov_len = strlen(buf[i]) + 1;
    }
    /* with a single call, write then all out */
    nr = writev(fd, iov, 3);
    if (nr < 0)
        err_sys("writev");
    printf("writev %zd bytes\n", nr);
    if (close(fd) < 0)
        err_sys("close");
    exit(0);
}
