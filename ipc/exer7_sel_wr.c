#include "myapue.h"
#include <sys/select.h>
#include <signal.h>

static void sig_pipe(int signo)
{
    printf("caught SIGPIPE\n");
}

int main(void)
{
    int retvalue, n, fd[2];
    pid_t pid;
    char line[MAXLINE];
    fd_set writefds;
    struct timeval timeout;

    if (signal(SIGPIPE, sig_pipe) == SIG_ERR)
        err_sys("signal(SIGPIPE) error");

    if (pipe(fd) < 0)
        err_sys("pipe error");
    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid > 0) { /* parent */
        close(fd[0]);
        FD_ZERO(&writefds);
        FD_SET(fd[1], &writefds);

        timeout.tv_sec = 10;
        timeout.tv_usec = 0;

        retvalue = select(fd[1]+1, NULL, &writefds, NULL, &timeout);
        if (retvalue == -1)
            err_sys("select error");
        if (retvalue > 0) {
            if (FD_ISSET(fd[1], &writefds)) {
                while((n = read(STDIN_FILENO, line, MAXLINE)) > 0)
                    if (write(fd[1], line, n) != n)
                        err_sys("write error");
            }
        } else {
            printf("no input\n");
        }
        close(fd[1]);
    } else { /* child */
        close(fd[1]);
        close(fd[0]);
    }
    exit(0);
}
