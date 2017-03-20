#include "signal1.h"

Sigfunc *signal1(int signo, Sigfunc *func)
{
    struct sigaction act, oact;

    act.sa_handler = func;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    if (signo == SIGALRM) { /* don't want SIGALRM restarted to
                               allow us set a timeout for I/O operations */
#ifdef SA_INTERRUPT
        act.sa_flags |= SA_INTERRUPT; /* SA_INTERRUPT not defined on MAC OS X */
#endif
    } else {
        act.sa_flags |= SA_RESTART; /* system calls interrupted by this signal
                                      are automatically restarted */
    }
    if (sigaction(signo, &act, &oact) < 0)
        return SIG_ERR;
    
    return oact.sa_handler;
}

Sigfunc *signal_inr(int signo, Sigfunc *func)
{
    struct sigaction act, oact;

    act.sa_handler = func;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
#ifdef SA_INTERRUPT
    act.sa_flags |= SA_INTERRUPT;
#endif
    if (sigaction(signo, &act, &oact) < 0)
        return SIG_ERR;
    return oact.sa_handler;
}

void abort1(void)
{
    sigset_t mask;
    struct sigaction action;

    /* Caller can't ignore SIGABRT, if so reset to default */
    sigaction(SIGABRT, NULL, &action);
    if (action.sa_handler == SIG_IGN) {
        action.sa_handler = SIG_DFL;
        sigaction(SIGABRT, &action, NULL);
    }
    if (action.sa_handler == SIG_DFL)
        fflush(NULL); /* flush all open stdio stream */

    /* Caller can't block SIGABRT, make sure it's unblocked */
    sigfillset(&mask);
    sigdelset(&mask, SIGABRT);
    sigprocmask(SIG_SETMASK, &mask, NULL);
    kill(getpid(), SIGABRT); /* send the signal */

    /* if we're here, process caught SIGABRT and returned */
    fflush(NULL);
    action.sa_handler = SIG_DFL; /* any other handler will be replaced to SIG_DFL */
    sigaction(SIGABRT, &action, NULL); /* reset to default */
    sigprocmask(SIG_SETMASK, &mask, NULL); /* just in case ... */
    kill(getpid(), SIGABRT);
    exit(1); /* this should never be executed ... */
}
