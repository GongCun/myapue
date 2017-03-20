#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "myapue.h"
#include "signal1.h"

static void sig_usr1(int);

int main(void)
{
    int n;
    char line[MAXLINE];
    
    printf("pid = %ld\n", (long)getpid());

    if (signal_inr(SIGUSR1, sig_usr1) == SIG_ERR)
        err_sys("signal_inr(SIGUSR1) error");

    if ((n = read(STDIN_FILENO, line, MAXLINE)) < 0)
        err_sys("read error");

    write(STDOUT_FILENO, line, n);

    return 0;
}

static void sig_usr1(int signo)
{ 
    return; /* nothing to do, just return to interrupt the read */
}
