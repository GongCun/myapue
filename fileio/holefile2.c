#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int fd;

    if ((fd = creat("test.hole", S_IRUSR|S_IWUSR|S_IRGRP)) < 0)
        exit(1);

    if (lseek(fd, 103316351, SEEK_SET) == -1)
        exit(1);
    if (write(fd, " ", 1) != 1)
        exit(1);

    return 0;
}
