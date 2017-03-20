#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

extern void get_fl(int);

int main(void)
{
    int fd;
    
    fd = dup2(0, 5);
    if (fd < 0) {
        perror("dup2");
        exit(1);
    }
    printf("fd = %d\n", fd);
    get_fl(fd);
    return 0;
}
