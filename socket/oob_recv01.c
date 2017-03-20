#include "myapue.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>

#define SERV "oobserv"

int listenfd, connfd;

void sig_urg(int);

int main(int argc, char **argv)
{
    int n;
    char *host;
    char buff[100];

    n = (int)sysconf(_SC_HOST_NAME_MAX);
    if (n < 0)
        err_sys("sysconf error");
    if ((host = malloc(n)) == NULL)
        err_sys("malloc error");
    if (gethostname(host, n) < 0)
        err_sys("gethostname error");

    listenfd = Tcp_listen(host, SERV, NULL);
    connfd = accept(listenfd, NULL, NULL);
    if (signal(SIGURG, sig_urg) == SIG_ERR)
        err_sys("signal(SIGURG) error");
    if (fcntl(connfd, F_SETOWN, getpid()) == -1)
        err_sys("fcntl F_SETOWN error");

    for (;;) {
        if ((n = read(connfd, buff, sizeof(buff)-1)) == 0) {
            printf("receive EOF\n");
            exit(0);
        }
        buff[n] = 0;
        printf("read %d bytes: %s\n", n, buff);
    }
}

void sig_urg(int signo)
{
    int n;
    char buff[100];

    printf("SIGURG received\n");
    if ((n = recv(connfd, buff, sizeof(buff)-1, MSG_OOB)) < 0)
        err_sys("recv error");
    buff[n] = 0;
    printf("read %d OOB byte: %s\n", n, buff);
}

