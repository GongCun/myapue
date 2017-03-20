#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define MAX_COUNT 1000000

int main(void)
{
    int fd;
    unsigned long i;
    char buf[16];
    size_t count;
    ssize_t n;

    fd = open("./exer6_count.txt", O_RDWR|O_APPEND);
    if (fd < 0)
        return -1;
    ftruncate(fd, 0);

    for (i = 0; i < MAX_COUNT; i++) {
        sprintf(buf, "%ld\n", i);
        count = strlen(buf) + 1;
        if ((n = write(fd, buf, count)) < 0)
            return -1;
        if (n != count) 
            return -1;
    }

    return 0;
}
