#include <stdlib.h>
#include <signal.h>
#include "myapue.h"

static void sig_alrm(int);

int main(void)
{
    if (signal(SIGALRM, sig_alrm) == SIG_ERR)
        err_sys("signal(SIGALRM) error");
    alarm(5);
    pause();

    return 0; /* should not be here */
}

static void sig_alrm(int signo)
{
    abort();
}

