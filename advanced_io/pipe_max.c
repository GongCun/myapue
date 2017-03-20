#include "myapue.h"
#include <unistd.h>
#include <limits.h>

int main(void)
{
    int i, fd[2];

#ifdef PIPE_BUF
    printf("PIPE_BUF = %d\n", PIPE_BUF);
#endif
    if (pipe(fd) < 0)
        err_sys("pipe error");
    set_fl(fd[1], O_NONBLOCK);

    for (i = 0 ; ; i++)  {
        if(write(fd[1], "a", 1) != 1) {
            err_ret("write error");
            break;
        }
    }
    printf("pipe_max = %d\n", i);
    exit(0);
}

