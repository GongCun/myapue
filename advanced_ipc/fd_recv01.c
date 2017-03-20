#include "myapue.h"
#include <sys/socket.h>

int main(void)
{
    int n, newfd;
    char buf[MAXLINE];
    pid_t pid;
    static int fd[2] = { -1, -1 };

    if (s_pipe(fd) < 0)
        err_sys("s_pipe error");
    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) { /* child */
        close(fd[0]);
        if (fd[1] != STDIN_FILENO && 
                dup2(fd[1], STDIN_FILENO) != STDIN_FILENO)
            err_sys("dup2 error to stdin");
        if (fd[1] != STDOUT_FILENO &&
                dup2(fd[1], STDOUT_FILENO) != STDOUT_FILENO)
            err_sys("dup2 error to stdout");
        if (execl("./fd_send01", "fd_send01", (char *)0) < 0)
            err_sys("execl error");
    }
    /* parent continue... */
    close(fd[1]);
    if (write(fd[0], "/etc/hosts", 10) != 10)
        err_sys("write error");

    newfd = recv_fd(fd[0], write);
    while ((n = read(newfd, buf, sizeof(buf))) > 0)
        if (write(STDOUT_FILENO, buf, n) != n)
            err_sys("write to stdout error");
    /* 
     * if (waitpid(pid, NULL, 0) < 0) 
     *    err_sys("waitpid error");
     */

    exit(0);
}

