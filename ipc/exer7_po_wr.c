#include "myapue.h"
#include <poll.h>
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
    struct pollfd writefds[1];

    if (signal(SIGPIPE, sig_pipe) == SIG_ERR)
        err_sys("signal(SIGPIPE) error");

    if (pipe(fd) < 0)
        err_sys("pipe error");
    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid > 0) { /* parent */
        close(fd[0]);
        writefds[0].fd = fd[1];
        writefds[0].events = POLLOUT;

        retvalue = poll(writefds, 1, 10 * 1000);
        if (retvalue == -1)
            err_sys("select error");
        if (retvalue > 0) {
            if (writefds[0].revents & POLLOUT) {
                while((n = read(STDIN_FILENO, line, MAXLINE)) > 0)
                    if (write(fd[1], line, n) != n) {
                        perror("write error");
                        break;
                    }
            } else if (writefds[0].revents & POLLHUP) {
                printf("POLLUP\n");
            }
        } else {
            printf("no input\n");
        }
        close(fd[1]);
    } else { /* child */
        close(fd[1]);
        sleep(5);
        close(fd[0]);
    }
    exit(0);
}
