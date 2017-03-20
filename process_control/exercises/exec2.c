#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include "myapue.h"


int main(int argc, char **argv)
{
    DIR *dir;
    FILE *file;
    struct dirent *de;
    char buf[32];
    int fd;

    memset(buf, 0, sizeof(buf));

    if (opendir("/Users/gongcunjust") == NULL) 
        err_sys("opendir error");

    dir = malloc(sizeof(DIR));
    file = fopen(argv[1], "r");
    if (!file)
        err_sys("fopen error");
    if (!fread(dir, sizeof(DIR), 1, file))
        err_sys("fread error");
    if (fclose(file))
        err_sys("fclose error");

    fd = dir->__dd_fd;
    printf("%d\n", fd);
    errno = 0;
    if (readdir(dir) == NULL && errno != 0)
        err_sys("readdir error");

    return 0;
}

