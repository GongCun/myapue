#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "myapue.h"

extern void exer9_pr_mask(const char *);
static void sig_hup(int), sig_int(int), sig_quit(int);

int main(void)
{
    sigset_t newmask, oldmask;
    
    if (signal(SIGHUP, sig_hup) == SIG_ERR) err_sys("signal(SIGHUP) error");
    if (signal(SIGINT, sig_int) == SIG_ERR) err_sys("signal(SIGINT) error");
    if (signal(SIGQUIT, sig_quit) == SIG_ERR) err_sys("signal(SIGQUIT) error");

    sigemptyset(&newmask);
    sigaddset(&newmask, SIGHUP);
    sigaddset(&newmask, SIGINT);
    sigaddset(&newmask, SIGQUIT);
    sigprocmask(SIG_BLOCK, &newmask, &oldmask);

    exer9_pr_mask("blocked ");
    sleep(5); /* waiting for the signal to test block */

    sigprocmask(SIG_SETMASK, &oldmask, NULL);
    exer9_pr_mask("unblocked ");

    sleep(5); /* waiting for the signal to test unblock */

    return 0;
}

static void sig_hup(int signo)
{
    printf("\ncaught SIGHUP\n");
    return;
}

static void sig_int(int signo)
{
    printf("\ncaught SIGINT\n");
    return;
}

static void sig_quit(int signo)
{
    printf("\ncaught SIGQUIT\n");
    return;
}

