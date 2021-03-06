#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "myapue.h"

volatile sig_atomic_t quitflag; /* set nonzero by signal handler */
static void sig_int(int signo) /* ont signal handler for SIGINT and SIGQUIT */
{
    if (signo == SIGINT)
        printf("\ninterrupt\n");
    else if (signo == SIGQUIT)
        quitflag = 1; /* set flag for main loop */
}

int main(void)
{
    sigset_t newmask, oldmask, zeromask, pendmask;

    if (signal(SIGINT, sig_int) == SIG_ERR)
        err_sys("signal(SIGINT) error");
    if (signal(SIGQUIT, sig_int) == SIG_ERR)
        err_sys("signal(SIGQUIT) error");

    sigemptyset(&zeromask);
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGQUIT);

    /*
     * block SIGQUIT and save current mask
     */
    if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)
        err_sys("SIG_BLOCK error");

    /* get pending signal */ 
    /* sleep(5); */
    pause();
    if (sigpending(&pendmask) < 0)
        err_sys("sigpending error");
    if (sigismember(&pendmask, SIGQUIT))
            printf("\nSIGQUIT pending\n");

    while (quitflag  == 0)
        sigsuspend(&zeromask);

    /*
     * SIGQUIT has been caught and is now blocked; do whatever.
     */
   quitflag = 0; 

   /* 
    * reset signal mask which unblocks SIGQUIT
    */
   if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
       err_sys("SIG_SETMASK error");

   return 0;
}
