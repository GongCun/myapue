#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include "myapue.h"
#include "ystem.h"

static void sig_int(int), sig_quit(int), sig_usr1(int), sig_chld(int);

int main(void)
{
    int status;

    if (signal(SIGINT, sig_int) == SIG_ERR)
        err_sys("signal(SIGINT) error");
    if (signal(SIGQUIT, sig_quit) == SIG_ERR)
        err_sys("signal(SIGQUIT) error");
    if (signal(SIGUSR1, sig_usr1) == SIG_ERR)
        err_sys("signal(SIGUSR1) error");
    if (signal(SIGCHLD, sig_chld) == SIG_ERR)
        err_sys("signal(SIGCHLD) error");

    if ((status = ystem("/bin/ed")) < 0)
        err_sys("ystem() error");

    raise(SIGINT); raise(SIGQUIT); raise(SIGUSR1); raise(SIGCHLD);

    return 0;
}

static void sig_int(int signo)
{
    printf("caught SIGINT\n");
}

static void sig_quit(int signo)
{
    printf("caught SIGQUIT\n");
}

static void sig_usr1(int signo)
{
    printf("caught SIGUSR1\n");
}

static void sig_chld(int signo)
{
    printf("caught SIGCHLD\n");
}
