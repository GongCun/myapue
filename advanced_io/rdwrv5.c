#include "myapue.h"
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/uio.h>

static ssize_t native_writev(int, const struct iovec *, int);

int main(int argc, char **argv)
{
    struct iovec iov[2];
    ssize_t nr;
    int fd, i, count;

    if (argc != 2) {
        fprintf(stderr, "%s <count>\n", argv[0]);
        exit(1);
    }
    count = atoi(argv[1]);
    char *buf[] = {"1234567890\n", "1234567890\n"};
    fd = open("temp.txt", O_RDWR|O_CREAT|O_TRUNC, FILE_MODE);
    if (fd < 0)
        err_sys("creat error");

    /* fill out three iovec structures */
    for (i = 0; i < 2; i++) {
        iov[i].iov_base = buf[i];
        iov[i].iov_len = strlen(buf[i]) + 1;
    }
    /* with a single call, write then all out */
    for (i = 0; i < count; i++) {
        nr = native_writev(fd, iov, 2);
        if (nr < 0)
            err_sys("writev");
    }

    if (close(fd) < 0)
        err_sys("close");
    exit(0);
}

static ssize_t native_writev(int fd, const struct iovec *iov, int count)
{
    ssize_t ret = 0;
    ssize_t nr = 0;
    int i;
    char buf[20], *ptr;

    ptr = buf;
    for(i = 0; i < count; i++) {
        nr += iov[i].iov_len;
        strcpy(ptr, iov[i].iov_base);
        ptr = buf + nr;
        *ptr = 0;
    }
    if ((ret = write(fd, buf, nr)) != nr)
        err_sys("write error");
    return(ret);
}
