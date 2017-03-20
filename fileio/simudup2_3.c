#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#define MODE S_IWUSR | S_IRUSR | S_IWGRP | S_IRGRP | S_IROTH
extern void get_fl(int);

int adup2(int fd1, int fd2)
{
    char buf[20];
    int val;
    int fd;

    if ((val = fcntl(fd1, F_GETFL, 0)) < 0) {
        perror("fcntl");
        exit(1);
    }

    if (fd2 == fd1)
        return fd2;

    sprintf(buf, "/dev/fd/%d", fd2);
    if ((fd = open(buf, O_CREAT|O_EXCL, 0)) < 0) {
        if (errno != EEXIST && errno != EBADF) { 
            perror("open");
            exit(1);
        }
    }
    close(fd);
    close(fd2);
    sprintf(buf, "/dev/fd/%d", fd1);
    fd = open(buf, val, 0);
    while (fd != -1 && fd < fd2) {
        sprintf(buf, "/dev/fd/%d", fd);
        fd = open(buf, val, 0);
    }
    if (fd == -1) {
        perror("open");
        exit(1);
    }
    return fd;
}
        
int main(int argc, char **argv)
{
    int fd;
    
    if (argc != 3) {
        printf("%s <fd1#> <fd2#>\n", argv[0]);
        exit(1);
    }
    fd = adup2(atoi(argv[1]), atoi(argv[2]));
    printf("orignnal fd = %s: ", argv[1]);
    get_fl(atoi(argv[1]));
    printf("fd = %d; ", fd);
    get_fl(fd);
    return 0;
}
