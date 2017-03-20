#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include "myapue.h"

void pr_mask(const char *str)
{
    sigset_t sigset;
    int errno_save;
    errno_save = errno; /* we can be called by signal handler */

    /* test signal set */
    sigfillset(&sigset);

    if (sigprocmask(SIG_SETMASK, &sigset, NULL) < 0) {
        err_ret("sigprocmask error");
    } else {
        printf("%s", str);
        if (sigismember(&sigset, SIGINT))
            printf(" SIGINT");
        if (sigismember(&sigset, SIGQUIT))
            printf(" SIGQUIT");
        if (sigismember(&sigset, SIGUSR1))
            printf(" SIGUSR1");
        if (sigismember(&sigset, SIGUSR2))
            printf(" SIGUSR2");

        /* remaining signals can go here */

        printf("\n");
    }

    errno = errno_save; /* restore errno */
}

