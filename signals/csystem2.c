#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include "myapue.h"

static void sig_int(int), sig_chld(int), sig_usr1(int);
extern void pr_mask(const char *);

int main(void)
{
    int status;

    if (signal(SIGINT, sig_int) == SIG_ERR)
        err_sys("signal(SIGINT) error");
    if (signal(SIGCHLD, sig_chld) == SIG_ERR)
        err_sys("signal(SIGCHLD) error");
    if (signal(SIGUSR1, sig_usr1) == SIG_ERR)
        err_sys("signal(SIGUSR1) error");

    pr_mask("before system(): ");
    raise(SIGCHLD);
    if ((status = system("/bin/ed")) < 0)
        err_sys("system() error");
    pr_mask("after system(): ");

    return 0;
}

static void sig_int(int signo)
{
    printf("caught SIGINT\n");
}

static void sig_chld(int signo)
{
    printf("caught SIGCHLD\n");
}

static void sig_usr1(int signo)
{
    printf("caught SIGUSR1\n");
}
