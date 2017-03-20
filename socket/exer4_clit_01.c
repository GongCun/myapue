#include "myapue.h"
#include <errno.h>
#include <netdb.h>
#include <sys/socket.h>

#define MAXADDRLEN 256
#define BUFLEN 128
#define SERV "psserv"

void print_ps(int sockfd)
{
    int n;
    char buf[BUFLEN];
    while ((n = recv(sockfd, buf, BUFLEN, 0)) > 0)
        write(STDOUT_FILENO, buf, n);
    if (n < 0)
        err_sys("recv error");
}

int main(void)
{
    int sockfd, n;
    char *host;

    n = (int)sysconf(_SC_HOST_NAME_MAX);
    if ((host = malloc(n)) == NULL)
        err_sys("malloc error");
    if (gethostname(host, n) < 0)
        err_sys("gethostname error");
    sockfd = Tcp_connect(host, SERV);
    print_ps(sockfd);
    exit(0);
}
