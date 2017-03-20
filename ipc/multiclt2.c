#include "myapue.h"
#include <sys/stat.h>
#include "fifo.h"

int main(int argc, char **argv)
{
    int readfifo, writefifo;
    size_t len;
    ssize_t n;
    char *ptr, fifoname[MAXLINE], buff[MAXLINE];
    pid_t pid;

    if (setvbuf(stdin, NULL, _IONBF, 0) == EOF)
        err_sys("setvbuf error");
    if (setvbuf(stdout, NULL, _IONBF, 0) == EOF)
        err_sys("setvbuf error");

    /* create FIFO with out PID as part of name */
    pid = getpid();
    snprintf(fifoname, sizeof(fifoname), "./fifo.%ld", (long)pid);
    if ((mkfifo(fifoname, FILE_MODE) < 0) && (errno != EEXIST))
        err_sys("can't create %s", fifoname);

    for ( ; ; ) {
        fputs("prompt> ", stdout);
        /* start buffer with pid and a blank */
        snprintf(buff, sizeof(buff), "%ld ", (long)pid);
        len = strlen(buff);
        ptr = buff + len;
        /* read pathname */
        if ((fgets(ptr, MAXLINE-len, stdin) == NULL) && ferror(stdin))
            err_sys("fgets error");
        if (strncmp(ptr, "quit", 4) == 0)
            break;
        if (strncmp(ptr, "\n", 1) == 0)
            continue;
        len = strlen(buff); /* fgets() guarantees null byte at end */
        /* open FIFO to server and write PID and pathname null bytes at end */
        if ((writefifo = open(SERV_FIFO, O_WRONLY, 0)) < 0)
            err_sys("open error");
        if (write(writefifo, buff, len) != len)
            err_sys("write error");
        /* now open our FIFO; blocks until server opens for writing */
        if ((readfifo = open(fifoname, O_RDONLY, 0)) < 0)
            err_sys("open error");
        /* read from IPC, write to standard output */
        while ((n = read(readfifo, buff, MAXLINE)) > 0)
            if (write(STDOUT_FILENO, buff, n) != n)
                err_sys("write error");
    }
    if (close(readfifo) < 0)
        err_sys("close error");
    if (unlink(fifoname) < 0)
        err_sys("unlink error");
    exit(0);
}


