#include "myapue.h"
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>

static void client(int, int);
static void server(int, int);
static unsigned char translate(unsigned char);

int main(int argc, char **argv)
{
    int fd1[2], fd2[2];
    pid_t pid;

    if (pipe(fd1) < 0 || pipe(fd2) < 0)
        err_sys("pipe error");

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) { /* child */
        close(fd1[1]);
        close(fd2[0]);
        server(fd1[0], fd2[1]);
        exit(0);
    } 

    /* parent continue... */
    close(fd1[0]);
    close(fd2[1]);
    client(fd2[0], fd1[1]);
    if (waitpid(pid, NULL, 0) < 0)
        err_sys("waitpid error");
    exit(0);
}

static void client(int readfd, int writefd)
{
    size_t len;
    ssize_t n;
    char buf[MAXLINE];

    /* read pathname */
    if (fgets(buf, MAXLINE, stdin) == NULL) {
        if (ferror(stdin))
            err_sys("fgets error");
        else
            err_quit("fgets exception failed");
    }
    len = strlen(buf);
    if (buf[len-1] == '\n') /* read from terminal */
        len--;
    if (write(writefd, buf, len) != len)
        err_sys("write pipe error");

    /* set block read */
    clr_fl(readfd, O_NONBLOCK);
    while ((n = read(readfd, buf, MAXLINE)) > 0)
        if (write(STDOUT_FILENO, buf, n) != n)
            err_sys("write error");
    return;
}

static void server(int readfd, int writefd)
{
    int i, fd;
    ssize_t n;
    char buf[MAXLINE];

    /* read pathname from IPC channel */
    if ((n = read(readfd, buf, MAXLINE)) <= 0)
        err_quit("read error");
    buf[n] = 0;

    if ((fd = open(buf, O_RDONLY)) < 0) {
        /* error: must tell clien */
        snprintf(buf+n, sizeof(buf)-n, ":can't open, %s\n",
                strerror(errno));
        n = strlen(buf);
        if (write(writefd, buf, n) != n)
            err_sys("write error");
    } else {
        /* open succeeded: copy file to IPC channel */
        while ((n = read(fd, buf, MAXLINE)) > 0) {
            for(i = 0; i < n; i++)
                buf[i] = translate(buf[i]);
            if(write(writefd, buf, n) != n)
                err_sys("write error");
        }
        close(fd);
    }
    return;
}

static unsigned char
translate(unsigned char c)
{
    if (isalpha(c))
        c = toupper(c);
    return c;
}
