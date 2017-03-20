#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(void)
{
    int n;
    int fd;
    char buf[32];

    if ((fd = open("tempfile", O_RDWR)) < 0) {
        perror("open error");
        exit(1);
    }

    if (unlink("tempfile") < 0) {
        perror("unlink");
        exit(1);
    }
    printf("file unlinked\n");

    while ((n = read(fd, buf, 32)) > 0) {
        if (write(STDOUT_FILENO, buf, n) != n) {
            if (errno != 0)
                perror("write error");
            else
                printf("write error\n");
            exit(1);
        }
    }
    printf("done\n");
    exit(0);
}
