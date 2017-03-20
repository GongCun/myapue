#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

extern void get_fl(int);

int adup2(int fd1, int fd2)
{
    char buf[20];
    int fd;

    if (close(fd2) < -1) {
        perror("close");
        exit(1);
    }

    sprintf(buf, "/dev/fd/%d", fd1);
    if ((fd = open(buf, 0, 0)) < 0) {
        perror("open");
        exit(1);
    }
    printf("dup fd = %d\n", fd);
    get_fl(fd);
    return 0;
}

int main(void)
{
    int fd;
    
    adup2(0,3);
    fd = dup2(0, 5);
    if (fd < 0) {
        perror("dup2");
        exit(1);
    }
    printf("fd = %d\n", fd);
    get_fl(fd);
    return 0;
}
