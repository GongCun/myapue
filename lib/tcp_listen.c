#include "myapue.h"
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int
tcp_listen(const char *host, const char *serv, socklen_t *addrlenp)
{
    int listenfd, err;
    const int on = 1;
    struct addrinfo hints, *res, *ressave;

    bzero(&hints, sizeof(struct addrinfo));
    hints.ai_flags = AI_PASSIVE;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = 0;
    hints.ai_addrlen = 0;
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

    if ((err = getaddrinfo(host, serv, &hints, &res)) != 0) {
        err_quit("tcp_listen %s, %s getaddrinfo error: %s",
                host, serv, gai_strerror(err));
    }
    ressave = res;

    do { 
        listenfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if (listenfd < 0)
            continue; /* ignore this one */
        if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, 
                    &on, sizeof(int)) < 0)
            err_sys("setsockopt error");
        if (bind(listenfd, res->ai_addr, res->ai_addrlen) == 0)
            break; /* success */
        close(listenfd); /* ignore this one */
    } while ((res = res->ai_next) != NULL);

    if (res == NULL)
        err_sys("tcp_listen error for %s, %s", host, serv);
    if (listen(listenfd, LISTENQ) < 0)
        err_sys("listen error");
    if (addrlenp) 
        *addrlenp = res->ai_addrlen; /* return size of protocol address */
    freeaddrinfo(ressave);
    return(listenfd);
}

int
Tcp_listen(const char *host, const char *serv, socklen_t *addrlenp)
{
    return(tcp_listen(host, serv, addrlenp));
}
