#include <setjmp.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

typedef void Sigfunc(int);
static jmp_buf env_alrm;

static void sig_alrm(int signo)
{
    longjmp(env_alrm, 1);
}

unsigned int sleep2(unsigned int nsecs)
{
    Sigfunc *astat;
    unsigned int unslept;
    astat = signal(SIGALRM, sig_alrm);
    if (astat == SIG_ERR)
        return(nsecs);
    if(setjmp(env_alrm) == 0) {
        alarm(nsecs); /* start the timer */
        /* sleep 10; no race condition */
        pause(); /* next caught signal wake up us */
    }
    unslept = alarm(0);
    signal(SIGALRM, astat); /* resume the SIGALRM */
    return(unslept); /* turn off timer, return unslept time */
}
