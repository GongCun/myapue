#include "myapue.h"
#include <sys/stat.h>
#include "fifo.h"

int main(int argc, char **argv)
{
    int readfifo, writefifo, dummyfd, fd;
    char *ptr, buff[MAXLINE+1], fifoname[MAXLINE];
    pid_t pid;
    ssize_t n;

    /* create server's well-known FIFO; OK if already exists */
    if (mkfifo(SERV_FIFO, FILE_MODE) < 0 && (errno != EEXIST))
        err_sys("can't create %s", SERV_FIFO);

    /* open server's well-known FIFO for reading and writing */
    if ((readfifo = open(SERV_FIFO, O_RDONLY)) < 0)
        err_sys("open error");
    if ((dummyfd = open(SERV_FIFO, O_WRONLY)) < 0)
        err_sys("open error");
    while ((n = readline(readfifo, buff, MAXLINE)) > 0) {
        if (buff[n-1] == '\n')
            n--; /* delete newline from readline() */
        buff[n] = 0; /* null terminate pathname */

        if ((ptr = strchr(buff, ' ')) == NULL) {
            err_msg("invalid request: %s", buff);
            continue;
        }
        *ptr++ = 0; /* null terminate PID, ptr = pathname */
        pid = atol(buff);
        snprintf(fifoname, sizeof(fifoname), "./fifo.%ld", (long)pid);
        if ((writefifo = open(fifoname, O_WRONLY)) < 0) {
            err_msg("can't open: %s", fifoname);
            continue;
        }
        if ((fd = open(ptr, O_RDONLY)) < 0) {
            /* error: must tell client */
            snprintf(buff + n, sizeof(buff) - n, ": can't open, %s\n",
                    strerror(errno));
            n = strlen(ptr);
            if (write(writefifo, ptr, n) != n)
                err_sys("write error");
            if (close(writefifo) < 0)
                err_sys("close error");
        } else {
            /* open succeeded: copy file to FIFO */
            while ((n = read(fd, buff, MAXLINE)) > 0)
                if (write(writefifo, buff, n) != n)
                    err_sys("write error");
            if (close(fd) < 0 || close(writefifo) < 0)
                err_sys("close error");
        }
    }
    exit(0);
}




