#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "myapue.h"
#include "signal1.h"

static void sig_alrm(int);

int main(void)
{
    int n;
    char line[MAXLINE];

    if (signal1(SIGALRM, sig_alrm) == SIG_ERR)
        err_sys("signal1(SIGALRM) error");

    alarm(10); /* 5 secs timeout */
    if ((n = read(STDIN_FILENO, line, MAXLINE)) < 0)
        err_sys("read error");
    alarm(0);

    write(STDOUT_FILENO, line, n);
    return 0;
}

static void sig_alrm(int signo)
{ 
    return; /* nothing to do, just return to interrupt the read */
}
