#include "myapue.h"
#include <signal.h>

static void sig_alrm(int);

int main(void)
{
    int n;
    char line[MAXLINE];
    struct sigaction act, oact;

    act.sa_handler = sig_alrm;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    act.sa_flags |= SA_RESTART;
    if (sigaction(SIGALRM, &act, &oact) < 0)
        err_sys("sigaction error");

    alarm(5);
    if ( (n = read(STDIN_FILENO, line, MAXLINE)) < 0)
        err_sys("read error");
    alarm(0);

    if ( write(STDOUT_FILENO, line, n) != n )
        err_sys("write error");
    printf("finished\n");
    exit(0);
}

static void sig_alrm(int signo)
{
    printf("caught SIGALRM\n");
    return;
}
