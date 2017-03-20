#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include "myapue.h"

DIR *dir;
void read_dir(DIR *dir)
{
    struct dirent *de;

    while ((de = readdir(dir)) != NULL)
        printf("%s\n", de->d_name);
}

int main(void)
{
    int dirfd, val, fval;
    int fd;
    pid_t pid;

    if ((dir = opendir("/Users/gongcunjust")) == NULL)
        err_sys("opendir error");

    dirfd = dir->__dd_fd;

    if ((val = fcntl(dirfd, F_GETFD, 0)) < 0)
        err_sys("fcntl error");
    printf("before dir val = %d\n", val);

    val |= FD_CLOEXEC;
    printf("after dir val = %d\n", val); /* as the same */
    closedir(dir);
    fputc('\n', stdout);

    fd = open("/Users/gongcunjust", O_RDONLY);
    if (fd < 0)
        err_sys("open error");
    if ((fval = fcntl(fd, F_GETFD, 0)) < 0)
        err_sys("fcntl error");
    printf("before fval = %d\n", fval);
    fval |= FD_CLOEXEC;
    printf("after fval = %d\n", fval);
    close(fd);

    return 0;
}

