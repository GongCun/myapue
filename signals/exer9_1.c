#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>

static void sig_int(int);

int main(void)
{
    sigset_t newmask, oldmask, pendmask;

    if (signal(SIGINT, sig_int) == SIG_ERR) {
        perror("signal(SIGINT)");
        return -1;
    }

    /*
     * block signal and save current signal mask
     */
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGHUP);
    sigaddset(&newmask, SIGINT);
    sigaddset(&newmask, SIGQUIT);
    if(sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0) {
        perror("SIG_BLOCK");
        return -1;
    }

    sleep(5); /* block signals will remain pending */

    if (sigpending(&pendmask) < 0) {
        perror("sigpending");
        return -1;
    }

    if (sigismember(&pendmask, SIGHUP))
        printf("\nSIGHUP pending\n");
    if (sigismember(&pendmask, SIGINT))
        printf("\nSIGINT pending\n");
    if (sigismember(&pendmask, SIGQUIT))
        printf("\nSIGQUIT pending\n");
    if (sigismember(&pendmask, SIGILL))
        printf("\nSIGILL pending\n");

    /* 
     * reset signal mask
     */
    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0) {
        perror("SIG_SETMASK");
        return -1;
    }
    printf("signals unblocked\n");

    if (sigpending(&pendmask) < 0) {
        perror("sigpending");
        return -1;
    }
    if (sigismember(&pendmask, SIGHUP))
        printf("\nSIGHUP pending\n");

    sleep(5);

    return 0;
}

static void sig_int(int signo)
{
    printf("\ncaught SIGINT\n");
    return;
}
