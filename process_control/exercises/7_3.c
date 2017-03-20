#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include "myapue.h"


int main(void)
{
    DIR *dir, *dir1;
    pid_t pid;
    FILE *file;
    int nr;
    struct dirent *de;

    dir1 = malloc(sizeof(DIR));

    if ((dir = opendir("/Users/gongcunjust")) == NULL) 
        err_sys("opendir error");

    file = fopen("/Users/gongcunjust/c_program/sys_program/process_control/exercises/DIR", "w");
    if (!file)
        err_sys("fopen error");
    if (!fwrite(dir, sizeof(DIR), 1, file))
        err_sys("fwrite error");

    if (fclose(file))
        err_sys("fclose error");

    /*
    file = fopen("/Users/gongcunjust/c_program/sys_program/process_control/exercises/DIR", "r");
    if (!file)
        err_sys("fopen error");
    if (!fread(dir1, sizeof(DIR), 1, file))
        err_sys("fread error");
    if (fclose(file))
        err_sys("fclose error");

    while ((de = readdir(dir1)) != NULL)
        printf("%s\n", de->d_name);
        */
    if (execl("/Users/gongcunjust/c_program/sys_program/process_control/exercises/exec1", 
           "exec1", "/Users/gongcunjust/c_program/sys_program/process_control/exercises/DIR", (char *)0) < 0) 
        err_sys("execl error");

    exit(0);
}

