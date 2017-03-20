#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <time.h>

int main(int argc, char **argv)
{
    int n;
    int fd;
    char buf[32];
    struct stat statbuf;
    struct tm *tmptr;

    char *TEMPFILE = argv[1];

    if ((fd = open(TEMPFILE, O_RDWR)) < 0) {
        perror("open error");
        exit(1);
    }

    if (fstat(fd, &statbuf) < 0) {
        perror("fstat error");
        exit(1);
    }
    if ((tmptr = localtime(&statbuf.st_ctime)) == NULL) {
        perror("localtime error");
        exit(1);
    }
    printf("st_ctime = %d:%d:%d\n", tmptr->tm_hour, tmptr->tm_min, tmptr->tm_sec);

    if (unlink(TEMPFILE) < 0) {
        perror("unlink");
        exit(1);
    }
    printf("file unlinked\n");

    if (fstat(fd, &statbuf) < 0) {
        perror("fstat error");
        exit(1);
    }
    if ((tmptr = localtime(&statbuf.st_ctime)) == NULL) {
        perror("localtime error");
        exit(1);
    }
    printf("st_ctime = %d:%d:%d\n", tmptr->tm_hour, tmptr->tm_min, tmptr->tm_sec);

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
