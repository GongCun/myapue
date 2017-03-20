#include "myapue.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>

#define SERV "oobserv"


int main(void)
{
    int sockfd, n;
    char *host;

    n = (int)sysconf(_SC_HOST_NAME_MAX);
    if (n < 0)
        err_sys("sysconf error");
    if ((host = malloc(n)) == NULL)
        err_sys("malloc error");
    if (gethostname(host, n) < 0)
        err_sys("gethostname error");

    sockfd = Tcp_connect(host, SERV);

    if (write(sockfd, "123", 3) != 3)
        err_sys("write error");
    printf("wrote 3 bytes of normal data\n");

    if (send(sockfd, "4", 1, MSG_OOB) != 1)
        err_sys("send error");
    printf("wrote 1 byte of OOB data\n");

    if (send(sockfd, "4", 1, MSG_OOB) != 1)
        err_sys("send error");
    printf("wrote 1 byte of OOB data\n");

    if (write(sockfd, "5", 1) != 1)
        err_sys("write error");
    printf("wrote 1 byte of normal data\n");

    exit(0);
}
