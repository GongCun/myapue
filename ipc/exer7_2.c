#include "myapue.h"
#include <sys/select.h>

int main(void)
{
    int retvalue, n, fd[2];
    pid_t pid;
    char line[MAXLINE];
    fd_set readfds;
    struct timeval timeout;

    if (pipe(fd) < 0)
        err_sys("pipe error");
    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) { /* child */
        close(fd[1]);
        FD_ZERO(&readfds);
        FD_SET(fd[0], &readfds);

        timeout.tv_sec = 20;
        timeout.tv_usec = 0;

        retvalue = select(fd[0]+1, &readfds, NULL, NULL, &timeout);
        if (retvalue == -1)
            err_sys("select error");
        if (retvalue > 0) {
            if (FD_ISSET(fd[0], &readfds)) {
                while((n = read(fd[0], line, MAXLINE)) > 0)
                    if (write(STDOUT_FILENO, line, n) != n)
                        err_sys("write error");
                fputc('\n', stdout);
            }
        } else {
            printf("no input\n");
        }
    } else { /* parent */
        printf("parent pid = %ld\n", (long)getpid());
        close(fd[0]);
        sleep(5);
        close(fd[1]);
    }
    exit(0);
}
