#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#define CRMODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH)
int main(void)
{
    int fd;

    if ((fd = open("test.file", O_RDWR|O_CREAT, CRMODE)) < 0)
        perror("open error");
    if (ftruncate(fd, 0) < 0)
        perror("ftruncate error");
    if (truncate("test.hole", 0) < 0)
        perror("truncate error");
    exit(0);
}
