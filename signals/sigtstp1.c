#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "myapue.h"

#define BUFFSIZE 1024

static void sig_tstp(int);

int main(void)
{
    int n;
    char buf[BUFFSIZE];

    /*
     * only catch SIGTSTP if we're running with job-control shell
     */
    if (signal(SIGTSTP, SIG_IGN) == SIG_DFL)
        signal(SIGTSTP, sig_tstp);

    while ((n = read(STDIN_FILENO, buf, BUFFSIZE)) > 0)
        if (write(STDOUT_FILENO, buf, n) != n)
            err_sys("write error");

    if (n < 0)
        err_sys("read error");

    return 0;
}

static void sig_tstp(int signo) /* signal handler for SIGTSTP */
{
    sigset_t mask;

    /* ... move cursor to lower left corner, reset tty mode ... */
    system("tput cup $(tput lines) 0");

    /*
     * unblock SIGTSTP, since it's blocked while we're handling it
     */
    sigemptyset(&mask);
    sigaddset(&mask, SIGTSTP);
    sigprocmask(SIG_UNBLOCK, &mask, NULL);

    signal(SIGTSTP, SIG_DFL); /* reset disposition to default */

    kill(getpid(), SIGTSTP); /* and send the signal to ourself */

    /* we won't return from the kill until we're continued */

    signal(SIGTSTP, sig_tstp); /* reestablish signal handler */

    /* ... reset tty mode, redraw screen ... */
    system("clear;tput cup 0 0");
}

