#include "myapue.h"
#include <signal.h>
#include <sys/time.h>
#include <termios.h>

static void sig_alrm(int);
static void tick(void);
static void sig_io(int);

int main(void)
{
    if (signal(SIGIO, sig_io) == SIG_ERR)
        err_sys("signal(SIGIO) error");
    if (fcntl(STDIN_FILENO, F_SETOWN, getpid()) < 0)
        err_sys("F_SETOWN error");
    set_fl(STDIN_FILENO, O_ASYNC);
    tick();
    for (;;) {
        pause();
    }
    exit(0);
}

static void sig_alrm(int signo)
{
    printf("caught SIGALRM\n");
    return;
}

static void sig_io(int signo)
{
    char buf[BUFSIZ];
    int n;

    if ( (n = read(STDIN_FILENO, buf, BUFSIZ)) < 0)
        err_sys("read error");
    buf[strlen(buf)-1] = 0;
    if ( write(STDOUT_FILENO, buf, n) != n )
        err_sys("write error");
    putchar('\n');
    if ( (n = read(STDIN_FILENO, buf, BUFSIZ)) < 0 )
        err_sys("read error");
    return;
}

static void tick(void)
{
    struct itimerval delay;
    int ret;

    if (signal(SIGALRM, sig_alrm) == SIG_ERR)
        err_sys("signal(SIGALRM) error");
    delay.it_value.tv_sec = 1;
    delay.it_value.tv_usec = 0;
    delay.it_interval.tv_sec = 1;
    delay.it_interval.tv_usec = 0;
    ret = setitimer(ITIMER_REAL, &delay, NULL);
    if (ret) {
        err_sys("setitimer");
    }
}


