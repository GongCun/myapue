#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include "myapue.h"

void clr_fl(int, int);
void get_fl(int);

int main(int argc, char **argv)
{
    int n;
    int bufsiz;
    char *buf;

    if (argc < 2) {
        printf("missing argument\n");
        exit(1);
    }
    bufsiz = atol(argv[1]);
    if (errno != 0) {
        perror("atol");
        exit(1);
    }
    buf = (char *)malloc(sizeof(bufsiz));
    if (buf == NULL)
        exit(1);

    get_fl(STDOUT_FILENO);
    clr_fl(STDOUT_FILENO, O_SYNC);
    get_fl(STDOUT_FILENO);
    while ((n = read(STDIN_FILENO, buf, bufsiz)) > 0)
        if (write(STDOUT_FILENO, buf, n) != n)
            err_sys("write error");

    if (n < 0)
        err_sys("read error");

    fsync(STDOUT_FILENO);
    get_fl(STDOUT_FILENO);

    return 0;
}

void clr_fl(int fd, int flags) /* flags are file status flags to turn on */
{
    int val;

    if ((val = fcntl(fd, F_GETFL, 0)) < 0)
        err_sys("fcntl F_GETFL error");

    val &= ~flags;
    if (fcntl(fd, F_SETFL, val) < 0)
        err_sys("fcntl F_SETFL error");
    return;
}


void get_fl(int fd)
{
    int val;

    if ((val = fcntl(fd, F_GETFL, 0)) < 0)
        err_sys("fcntl F_GETFL error for fd %d\n", fd);

    switch(val & O_ACCMODE) {
        case O_RDONLY:
            printf("read only");
            break;
        case O_WRONLY:
            printf("write only");
            break;
        case O_RDWR:
            printf("read write");
            break;
        default:
            err_dump("unknown access mode");
    }

    if (val & O_APPEND)
        printf(", append");
    if (val & O_NONBLOCK)
        printf(", nonblocking");
    if (val & O_SYNC)
        printf(", synchronous write");
    putchar('\n');

}

