#include <signal.h>
#include <unistd.h>
#include <string.h>

typedef void Sigfunc(int);
static void sig_alrm(int signo)
{
    char *buf = "signal handler\n";
    write(STDOUT_FILENO, buf, strlen(buf)+1);
    return; /* nothing to do, just return to wake up the pause */
}

unsigned int sleep1(unsigned int nsecs)
{
    Sigfunc *astat;
    unsigned int unslept;
    astat = signal(SIGALRM, sig_alrm);
    if (astat == SIG_ERR)
        return(nsecs);
    alarm(nsecs); /* start the timer */
    sleep(10); /* it will cause race condition */
    pause(); /* next caught signal wake up us */
    unslept = alarm(0);
    signal(SIGALRM, astat); /* resume the SIGALRM */

    /* 
     * default alarm exit 
     * alarm(nsecs); 
     * pause(); 
     */

    return(unslept); /* turn off timer, return unslept time */
}
