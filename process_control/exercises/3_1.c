#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include "myapue.h"

extern void pr_exit(int);

int main(void)
{
    pid_t pid;
    int status;

    if ((pid = fork()) < 0)
        err_sys("fork error");
    else if (pid == 0)
        exit(7);

    if (waitpid(pid, &status, 0) != pid)
        err_sys("waitpid error");
    pr_exit(status);

    if ((pid = fork()) < 0)
        err_sys("fork error");
    else if (pid == 0)
        abort(); /* generates SIGABRT */

    if (waitpid(pid, &status, 0) != pid)
        err_sys("waitpid error");
    pr_exit(status);

    if ((pid = fork()) < 0)
        err_sys("fork error");
    else if (pid == 0)
        status /= 0; /* divide by 0 generates SIGFPE */

    if (waitpid(pid, &status, 0) != pid)
        err_sys("waitpid error");
    pr_exit(status);

    return 0;
}
