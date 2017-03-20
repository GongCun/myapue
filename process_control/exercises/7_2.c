#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include "myapue.h"

extern DIR *dir;

int main(void)
{
    pid_t pid;

    if ((dir = opendir("/Users/gongcunjust")) == NULL) 
        err_sys("opendir error");

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {
        if (execl("/Users/gongcunjust/c_program/sys_program/process_control/exercises/exec1",
                "exec1", (char *)0) < 0)
            err_sys("execl error");
    }

    if (waitpid(pid, NULL, 0) != pid)
        err_sys("waitpid error");

    return 0;
}

