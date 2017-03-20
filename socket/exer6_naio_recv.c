#include "myapue.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <signal.h>
#include <sys/ioctl.h>

#define SERV "aioserv"

int listenfd, connfd;

int main(void)
{
    int n, on = 0;
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
    if (ioctl(connfd, FIOASYNC, &on) < 0)
        err_sys("ioctl FIOASYNC error");
    while ((n = read(connfd, buff, sizeof(buff)-1)) != 0) {
        if (n < 0)
            err_sys("read error");
        buff[n] = 0;
        printf("read %d bytes: %s\n", n, buff);
    }
    printf("read EOF\n");
    exit(0);
}
