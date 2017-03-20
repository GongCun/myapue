#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "myapue.h"

int main(void)
{
    pid_t pid;

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {
        if (execl("/Users/gongcunjust/c_program/sys_program/process_control/testinterp", "testinterp", "myarg1", "MY ARG2", (char *)0) < 0)
            err_sys("execlp error");
    }
    if (waitpid(pid, NULL, 0) < 0) /* parent */
        err_sys("waitpid error");

    return 0;
}
