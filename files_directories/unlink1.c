#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(void)
{
    if (open("tempfile", O_RDWR) < 0) {
        perror("open error");
        exit(1);
    }
    if (unlink("tempfile") < 0) {
        perror("unlink");
        exit(1);
    }
    printf("file unlinked\n");
    sleep(15);
    printf("done\n");
    exit(0);
}
