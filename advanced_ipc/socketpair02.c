#include "myapue.h"
#include <sys/socket.h>

int main(void)
{
    int n;
    char buf[MAXLINE];
    pid_t pid;
    static int fd[2] = { -1, -1 };

    if (s_pipe(fd) < 0)
        err_sys("s_pipe error");
    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) { /* child */
        close(fd[0]);
        while ((n = read(fd[1], buf, sizeof(buf))) > 0) {
            buf[n-1] = 0;
            strcpy(buf+strlen(buf), " (child)\n");
            n = strlen(buf);
            if (write(fd[1], buf, n) != n)
                err_sys("child write error");
        }
        exit(0);
    }
    /* parent continue... */
    close(fd[1]);
    while ((n = read(STDIN_FILENO, buf, sizeof(buf))) > 0) {
        if (write(fd[0], buf, n) != n)
            err_sys("parent write error");
        if ((n = read(fd[0], buf, sizeof(buf))) > 0) {
            buf[n-1] = 0;
            strcpy(buf+strlen(buf), " (parent)\n");
            n = strlen(buf);
            if (write(STDOUT_FILENO, buf, n) != n)
                err_sys("parent write error");
        }
    }
    close(fd[0]);
    if (waitpid(pid, NULL, 0) < 0)
        err_sys("waitpid error");

    exit(0);
}

