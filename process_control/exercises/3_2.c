#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include "myapue.h"

extern void pr_exit(int);

static void my_handler(int, siginfo_t *, void *);

int main(void)
{
    pid_t pid;
    int status;
    struct sigaction sa;

    if ((pid = fork()) < 0)
        err_sys("fork error");
    else if (pid == 0)
        exit(7);

    if (waitpid(pid, &status, 0) != pid)
        err_sys("waitpid error");
    pr_exit(status);

    if ((pid = fork()) < 0)
        err_sys("fork error");
    else if (pid == 0) {
        abort(); /* generates SIGABRT */
    }

    if (waitpid(pid, &status, 0) != pid)
        err_sys("waitpid error");

    printf("signo = %d %s\n", WTERMSIG(status), strsignal(WTERMSIG(status)));

    if ((pid = fork()) < 0)
        err_sys("fork error");
    else if (pid == 0)
        status /= 0; /* divide by 0 generates SIGFPE */

    if (waitpid(pid, &status, 0) != pid)
        err_sys("waitpid error");

    printf("signo = %d %s\n", WTERMSIG(status), strsignal(WTERMSIG(status)));

    return 0;
}

static void my_handler(int signo, siginfo_t *si, void *ucontext)
{
    printf("si_signo = %d\n", si->si_signo);
}
