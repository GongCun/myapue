#include "myapue.h"
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/uio.h>

int main(int argc, char **argv)
{
    struct iovec iov[2];
    ssize_t nr;
    int fd, i, j, count;
    char buf[2][MAXLINE];

    if (argc != 2) {
        fprintf(stderr, "%s <count>\n", argv[0]);
        exit(1);
    }
    count = atoi(argv[1]);
    for (i = 0; i < 2; i++) {
       for (j = 0; j < MAXLINE-1; j++) 
           buf[i][j] = 'a';
       buf[i][j] = 0;
    }
    fd = open("temp.txt", O_RDWR|O_CREAT|O_TRUNC, FILE_MODE);
    if (fd < 0)
        err_sys("creat error");
    if (unlink("temp.txt") < 0)
        err_sys("unlink error");

    /* fill out three iovec structures */
    for (i = 0; i < 2; i++) {
        iov[i].iov_base = buf[i];
        iov[i].iov_len = strlen(buf[i]) + 1;
    }
    /* with a single call, write then all out */
    for (i = 0; i < count; i++) {
        nr = writev(fd, iov, 2);
        if (nr < 0)
            err_sys("writev");
    }

    if (close(fd) < 0)
        err_sys("close");
    exit(0);
}
