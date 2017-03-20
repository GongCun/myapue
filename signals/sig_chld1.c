#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "myapue.h"

static void sig_chld(int signo)
{
    printf("caught SIGCHLD\n");
}

int main(void)
{
    pid_t pid;

    if (signal(SIGCHLD, sig_chld) == SIG_ERR)
        err_sys("signal(SIGCHLD) error");

    if ((pid = fork()) < 0)
        err_sys("fork error");
    else if (pid == 0) {
        printf("child process\n");
        exit(0);
    }
    if (waitpid(pid, NULL, 0) != pid)
        err_sys("waitpid error");
    return 0;
}
