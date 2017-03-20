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

    dir = malloc(sizeof(DIR));
    file = fopen("/Users/gongcunjust/c_program/sys_program/process_control/exercises/DIR", "r");
    if (!file)
        err_sys("fopen error");
    if (!fread(dir, sizeof(DIR), 1, file))
        err_sys("fread error");
    if (fclose(file))
        err_sys("fclose error");

    printf("%d\n", dir->__dd_fd);

    errno = 0;
    if (readdir(dir) == NULL && errno != 0)
        err_sys("readdir error");
    /*
    while ((de = readdir(dir)) != NULL)
        printf("%s\n", de->d_name); */

    return 0;
}

