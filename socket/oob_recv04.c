#include "myapue.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>

#define SERV "oobserv"

int listenfd, connfd;

int main(int argc, char **argv)
{
    int n, on = 1;
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
    if (setsockopt(listenfd, SOL_SOCKET, SO_OOBINLINE, &on, sizeof(int)) < 0)
        err_sys("setsockopt error");
    connfd = accept(listenfd, NULL, NULL);
    /* 
     * to let all the data from sender be received.
     * this allows us to demonstrate that a "read" stops at
     * the out-of-band mark, even though additional data is
     * in the socket receive buffer
     */
    sleep(5); 

    for (;;) {
        if (sockatmark(connfd) == 1)
            printf("at OOB mark\n");
        if ((n = read(connfd, buff, sizeof(buff)-1)) == 0) {
            printf("receive EOF\n");
            exit(0);
        }
        buff[n] = 0;
        printf("read %d bytes: %s\n", n, buff);
    }
}
