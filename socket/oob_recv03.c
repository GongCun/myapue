#include "myapue.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/select.h>

#define SERV "oobserv"

int listenfd, connfd;

int main(int argc, char **argv)
{
    int n;
    char *host;
    char buff[100];
    fd_set rset, xset;
    int justreadoob;

    n = (int)sysconf(_SC_HOST_NAME_MAX);
    if (n < 0)
        err_sys("sysconf error");
    if ((host = malloc(n)) == NULL)
        err_sys("malloc error");
    if (gethostname(host, n) < 0)
        err_sys("gethostname error");

    listenfd = Tcp_listen(host, SERV, NULL);
    connfd = accept(listenfd, NULL, NULL);

    FD_ZERO(&rset);
    FD_ZERO(&xset);
    justreadoob = 0;

    for (;;) { 
        FD_SET(connfd, &rset);
        if (justreadoob == 0) 
            FD_SET(connfd, &xset);
        if (select(connfd+1, &rset, NULL, &xset, NULL) < 0) /* block indefinitely */
            err_sys("select error");
        if (FD_ISSET(connfd, &xset)) {
            n = recv(connfd, buff, sizeof(buff)-1, MSG_OOB);
            if (n < 0)
                err_sys("recv error");
            buff[n] = 0;
            printf("read %d OOB byte: %s\n", n, buff);
            justreadoob = 1;
            FD_CLR(connfd, &xset);
        }
        if (FD_ISSET(connfd, &rset)) {
            n = read(connfd, buff, sizeof(buff)-1);
            if (n < 0)
                err_sys("read error");
            if (n == 0) {
                printf("received EOF\n");
                exit(0);
            }
            buff[n] = 0;
            printf("read %d bytes: %s\n", n, buff);
            justreadoob = 0;
        }
    }
}
