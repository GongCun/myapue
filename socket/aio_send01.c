#include "myapue.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>

#define SERV "aioserv"


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
    sleep(1);

    if (write(sockfd, "45", 2) != 2)
        err_sys("write error");
    printf("wrote 2 bytes of normal data\n");
    sleep(1);

    if (write(sockfd, "67", 2) != 2)
        err_sys("write error");
    printf("wrote 2 bytes of normal data\n");
    sleep(1);

    exit(0);
}
