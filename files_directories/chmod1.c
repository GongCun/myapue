#include <sys/stat.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    struct stat statbuf;

    /* turn on set-group-id and turn off group-execute */
    if (stat("foo", &statbuf) < 0) {
        perror("stat error for foo");
        exit(1);
    }
    if (chmod("foo", (statbuf.st_mode & ~S_IXGRP) | S_ISGID) < 0) {
        perror("chmod error for foo");
        exit(1);
    }

    /* set absolute mode to "rw-r--r--" */
    if (chmod("bar", S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH) < 0) {
        perror("chmod error for bar");
        exit(1);
    }

    exit(0);
}

