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
    struct pollfd readfds[1];

    if (signal(SIGPIPE, sig_pipe) == SIG_ERR)
        err_sys("signal(SIGPIPE) error");

    if (pipe(fd) < 0)
        err_sys("pipe error");
    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) { /* child */
        close(fd[1]);
        readfds[0].fd = fd[0];
        readfds[0].events = POLLIN;

        retvalue = poll(readfds, 1, 10 * 1000);
        if (retvalue == -1)
            err_sys("select error");
        if (retvalue > 0) {
            if (readfds[0].revents & POLLIN) {
                while((n = read(fd[0], line, MAXLINE)) > 0)
                    if (write(STDOUT_FILENO, line, n) != n)
                        err_sys("write error");
                fputc('\n', stdout);
            }
            if (readfds[0].revents & POLLHUP) {
                printf("POLLUP\n");
            }
        } else {
            printf("no input\n");
        }
        close(fd[0]);
    } else { /* parent */
        close(fd[0]);
        sleep(5);
        close(fd[1]);
    }
    exit(0);
}
