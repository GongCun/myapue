#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define BUFFSIZE 100
#define TARGETF "./exer11_target.file"
#define SOURCEF "./exer11_source.file"

typedef void Sigfunc(int);

Sigfunc *signal_intr(int signo, Sigfunc *func)
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

static void sig_xfsz(int);

int main(void)
{
    int n;
    int wn;
    char buf[BUFFSIZE];
    int fds, fdt;

    if (signal_intr(SIGXFSZ, sig_xfsz) == SIG_ERR)
    {
        printf("signal(SIGXFSZ) error");
        return -1;
    }

    fds = open(SOURCEF, O_RDONLY, 0);
    if (fds < 0)
        return -1;
    fdt = open(TARGETF, O_WRONLY|O_APPEND, 0);
    if (fdt < 0)
        return -1;

    while ((n = read(fds, buf, BUFFSIZE)) > 0)
        if ((wn = write(fdt, buf, n)) < n) 
        {
            if (wn < 0) 
            {
                perror("write");
                return -1;
            }
            printf("written bytes %d, expected bytes %d\n", wn, n);
            return -1;
        }

    if (n < 0) 
    {
        printf("read error\n");
        return -1;
    }

    return 0;
}

static void sig_xfsz(int signo)
{
    printf("caught SIGXFSZ\n");
    return;
}

