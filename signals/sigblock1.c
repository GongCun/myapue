#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

static void sig_int(int);

int main(void)
{
    sigset_t newmask, oldmask;
    
    if (signal(SIGINT, sig_int) == SIG_ERR)
        return -1;
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGINT);
    if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)
        return -1;

    sleep(30); /* wait for the SIGINT */

    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
        return -1;

    return 0;
}

static void sig_int(int signo)
{
    printf("caught SIGINT\n");
}

