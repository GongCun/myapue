#include "myapue.h"
#include <unistd.h>
#include <limits.h>
#include <signal.h>

static void sig_pipe(int signo)
{
    /* only caught the SIGPIPE */
    return;
}

int main(void)
{
    int i, fd[2];

    if (signal(SIGPIPE, sig_pipe) == SIG_ERR)
        err_sys("signal(SIGPIPE) error");
    if (pipe(fd) < 0)
        err_sys("pipe error");
    printf("PIPE_BUF = %ld\n", fpathconf(fd[1], _PC_PIPE_BUF));
    set_fl(fd[1], O_NONBLOCK);
    if (close(fd[0]) < 0)
        err_sys("close error");

    for (i = 0 ; ; i++)  {
        if(write(fd[1], "a", 1) != 1) {
            printf("errno = %d\n", errno);
            err_ret("write error");
            break;
        }
    }
    printf("pipe_max = %d\n", i);
    exit(0);
}

