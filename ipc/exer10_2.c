#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include "myapue.h"

#define FIFO "temp.fifo"

int main(void)
{
    int fd, dummyfd, n;
	struct timeval timeout;
	fd_set readfds;
	int maxfd;
	int retval;
    char line[MAXLINE];


    if ((mkfifo(FIFO, FILE_MODE) < 0) && (errno != EEXIST))
       err_sys("mkfifo error");

    if ((fd = open(FIFO, O_RDONLY|O_NONBLOCK)) < 0)
        err_sys("open for read error");
    /*
    if ((dummyfd = open(FIFO, O_WRONLY)) < 0)
        err_sys("open for write error");
    clr_fl(fd, O_NONBLOCK);
    */
    maxfd = 1 + fd;

    while (1) {
        /* 
         * make a list of file descriptors
         */
        FD_ZERO(&readfds);
        FD_SET(fd, &readfds);

        /*
         * set timeout value
         */
        timeout.tv_sec = 2;
        timeout.tv_usec = 0;

        /* wait for input */
        retval = select(maxfd, &readfds, NULL, NULL, &timeout);
        if (retval == -1)
            err_sys("select", 4);
        if (retval > 0) {
            if (FD_ISSET(fd, &readfds)) {
                while((n = read(fd, line, MAXLINE)) > 0)
                    if (write(STDOUT_FILENO, line, n) != n)
                        err_sys("write error");
            }
        } else {
            printf("no input\n"); 
        }
    }

    exit(0);
}
