#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(void)
{
    int fd;

    if ( (fd = open("/dev/tty", O_RDWR)) < 0 ) {
        perror("open error");
        exit(1);
    }
    if (write(fd, "abcde", 5) != 5) {
        perror("write error");
        exit(1);
    }
    putchar('\n');
    fflush(stdout);
    exit(0);
}
