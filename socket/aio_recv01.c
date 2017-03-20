#include "myapue.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <signal.h>
#include <sys/ioctl.h>

#define SERV "aioserv"

volatile sig_atomic_t quitflag;
int listenfd, connfd;

void sig_io(int);

int main(int argc, char **argv)
{
    int n, on = 1;
    char *host;
    char buff[100];
    sigset_t  zeromask, newmask, oldmask;

    n = (int)sysconf(_SC_HOST_NAME_MAX);
    if (n < 0)
        err_sys("sysconf error");
    if ((host = malloc(n)) == NULL)
        err_sys("malloc error");
    if (gethostname(host, n) < 0)
        err_sys("gethostname error");

    listenfd = Tcp_listen(host, SERV, NULL);
    connfd = accept(listenfd, NULL, NULL);
    if (fcntl(connfd, F_SETOWN, getpid()) == -1)
        err_sys("fcntl F_SETOWN error");
    if (ioctl(connfd, FIOASYNC, &on) < 0)
        err_sys("ioctl FIOASYNC error");
    if (ioctl(connfd, FIONBIO, &on) < 0)
        err_sys("ioctl FIONBIO error");

    if (signal(SIGIO, sig_io) == SIG_ERR)
        err_sys("signal(SIGIO) error");
    if (sigemptyset(&zeromask) != 0)
        err_sys("sigemptyset error");
    if (sigemptyset(&newmask) != 0)
        err_sys("sigemptyset error");
    if (sigemptyset(&oldmask) != 0)
        err_sys("sigemptyset error");
    if (sigaddset(&newmask, SIGIO) != 0)
        err_sys("sigaddset error");

    if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)
        err_sys("sigprocmask error");

    while (quitflag == 0)
        sigsuspend(&zeromask);
    quitflag = 0; 
    printf("receive EOF\n");
    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
        err_sys("sigprocmask error");
    exit(0);
}

void sig_io(int signo)
{
    int n;
    char buff[100];
    printf("SIGIO received\n");
AGAIN:
    if ((n = read(connfd, buff, sizeof(buff)-1)) == 0) {
        quitflag = 1;
        return;
    } 
    if (n < 0) { 
        if (errno == EAGAIN || errno == EWOULDBLOCK) 
            goto AGAIN; 
        else 
            err_sys("read error"); 
    }
    buff[n] = 0;
    printf("read %d bytes: %s\n", n, buff);
    return;
}


