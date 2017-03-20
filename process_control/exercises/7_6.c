#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include "myapue.h"

int main(void)
{
    DIR *dir;
    int dirfd, val, fval, fd;
    char buf[MAXLINE];
    pid_t pid;

    if ((dir = opendir("/Users/gongcunjust")) == NULL)
        err_sys("opendir error");

    dirfd = dir->__dd_fd;
    printf("dirfd = %d\n", dirfd);
    if ((val = fcntl(dirfd, F_GETFD, 0)) < 0)
        err_sys("fcntl error");
    printf("FD_CLOEXEC = %d\n", val);

    if (fcntl(dirfd, F_SETFD, 0) < 0)
        err_sys("F_SETFD error");
    if ((val = fcntl(dirfd, F_GETFD, 0)) < 0)
        err_sys("fcntl error");
    printf("FD_CLOEXEC = %d\n", val);

    if ( (fval = fcntl(dirfd, F_GETFL, 0)) < 0 )
        err_sys("fcntl error");

    switch(fval & O_ACCMODE) {
        case O_RDONLY:
            printf("read only");
            break;
        case O_WRONLY:
            printf("write only");
            break;
        case O_RDWR:
            printf("read write");
            break;

        default:
            err_dump("unknown access mode");
    }
    if (fval & O_APPEND)
        printf(", append");
    if (fval & O_NONBLOCK)
        printf(", nonblocking");
    if (fval & O_SYNC)
        printf(", synchronous writes");
    putchar('\n');

    if ( (fd = open("/Users/gongcunjust", O_RDONLY)) < 0 ) /* directory open read only */
        err_sys("open error");
    
    if ( (pid = fork()) < 0 ) {
        err_sys("fork error");
    } else if (pid == 0) { /* test the FD_CLOEXEC */
        sprintf(buf, "%d", dirfd);
        if (execl("/Users/gongcunjust/c_program/sys_program/process_control/exercises/pr_flags1",
                    "pr_flags1", buf, (char *)0) < 0)
            err_ret("execl error");
    }

    if (waitpid(pid, NULL, 0) < 0)
        err_sys("wait error");

    if ( (pid = fork()) < 0 ) {
        err_sys("fork error");
    } else if (pid == 0) { /* test the FD_CLOEXEC */
        sprintf(buf, "%d", fd);
        if (execl("/Users/gongcunjust/c_program/sys_program/process_control/exercises/pr_flags1",
                    "pr_flags1", buf, (char *)0) < 0)
            err_ret("execl error");
    }
    if (waitpid(pid, NULL, 0) < 0)
        err_sys("wait error");


    return 0;
}
