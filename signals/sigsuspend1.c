#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "myapue.h"

static void sig_int(int);
static void sig_usr1(int);
extern void pr_mask(const char *);

int main(void)
{
    sigset_t newmask, oldmask, waitmask;

    pr_mask("program start: ");

    if (signal(SIGINT, sig_int) == SIG_ERR)
        err_sys("signal(SIGINT) error");
    if (signal(SIGUSR1, sig_usr1) == SIG_ERR)
        err_sys("signal(SIGUSR1) error");

    sigemptyset(&waitmask);
    sigaddset(&waitmask, SIGUSR1);
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGINT);

    /*
     * block SIGINT and save current signal mask
     */
    if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)
        err_sys("SIG_BLOCk error");

    /*
     * critical region of code
     */
    pr_mask("in critical region: ");

    /*
     * pause, allowing all signals except SIGUSR1
     */
    if (sigsuspend(&waitmask) != -1)
        err_sys("sigsuspend error");
    pr_mask("after return from suspend: "); /* restores the signal mask to
                                               its values before the call */

    /*
     * reset signal mask which unblocks SIGINT
     */
    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
        err_sys("SIG_SETMASK error");

    /*
     * and continue processing ...
     */
    pr_mask("program exit: ");

    return 0;
}

static void sig_int(int signo)
{
    pr_mask("\nin sig_int: ");
}

static void sig_usr1(int signo)
{
    pr_mask("\nin sig_usr1: ");
}
