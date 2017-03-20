#include <sys/stat.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    struct stat statbuf;

    if (stat("sticky.file", &statbuf) < 0) {
        perror("stat error for sticky.file");
        exit(1);
    }
    if (chmod("sticky.file", statbuf.st_mode | S_ISVTX) < 0) {
        perror("chmod error for foo");
        exit(1);
    }

    exit(0);
}

