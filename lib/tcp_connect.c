#include "myapue.h"
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int
tcp_connect(const char *host, const char *serv)
{
    int sockfd, err;
    struct addrinfo hints, *res, *ressave;

    bzero(&hints, sizeof(struct addrinfo));
    hints.ai_flags = AI_CANONNAME;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = 0;
    hints.ai_addrlen = 0;
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

    if ((err = getaddrinfo(host, serv, &hints, &res)) != 0) {
        err_quit("tcp_connect %s, %s getaddrinfo error: %s",
                host, serv, gai_strerror(err));
    }
    ressave = res;

    do { 
        sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if (sockfd < 0)
            continue; /* ignore this one */
        if (connect(sockfd, res->ai_addr, res->ai_addrlen) == 0)
            break;
        close(sockfd); /* ignore this one */
    } while ((res = res->ai_next) != NULL);

    if (res == NULL)
        err_sys("tcp_connect error for %s, %s", host, serv);
    freeaddrinfo(ressave);
    return(sockfd);
}

int
Tcp_connect(const char *host, const char *serv)
{
    return(tcp_connect(host, serv));
}
