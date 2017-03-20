#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include "myapue.h"

void set_fl(int fd, int flags) /* flags are file status flags to turn on */
{
    int val;

    if ((val = fcntl(fd, F_GETFL, 0)) < 0)
        err_sys("fcntl F_GETFL error");

    val |= flags;
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

int main(int argc, char **argv)
{

    if(argc != 2)
        err_quit("missing argument");

    get_fl(atoi(argv[1]));

    set_fl(atoi(argv[1]), O_APPEND);
    get_fl(atoi(argv[1]));
    set_fl(atoi(argv[1]), O_NONBLOCK);
    get_fl(atoi(argv[1]));
    set_fl(atoi(argv[1]), O_SYNC);
    get_fl(atoi(argv[1]));
    set_fl(atoi(argv[1]), O_DSYNC);
    get_fl(atoi(argv[1]));
    set_fl(atoi(argv[1]), O_FSYNC);
    get_fl(atoi(argv[1]));
    set_fl(atoi(argv[1]), O_ASYNC);
    get_fl(atoi(argv[1]));

    return 0;
}

