#include "open.h"
#include <poll.h>

void loop_poll(void)
{
    int i, n, listenfd, clifd, nread;
    char buf[MAXLINE];
    uid_t uid;
    struct pollfd *pollfd;

    if ((pollfd = malloc(open_max() * sizeof(struct pollfd))) == NULL)
        err_sys("malloc error");
    
    /* obtain fd to listen for client request on */
    if ((listenfd = Serv_listen(CS_OPEN)) < 0)
        log_sys("serv_listen error");
    client_add(listenfd, 0); /* we use [0] for listenfd */
    pollfd[0].fd = listenfd;
    pollfd[0].events = POLLIN;
    n = 0;

    for (;;) {
        if ((n = poll(pollfd, n+1, -1)) < 0) /* wait forever */
            log_sys("poll error");
        
        if (pollfd[0].revents & POLLIN) {
            /* accept new client request */
            if ((clifd = Serv_accept(listenfd, &uid)) < 0)
                log_sys("serv_accept error: %d", clifd);
            i = client_add(clifd, uid);
            pollfd[i].fd = clifd;
            pollfd[i].events = POLLIN;
            log_msg("new connection: uid %d, fd %d", uid, clifd);
        }

        for (i = 1; i <= n; i++) { /* [0] is listenfd */
            if ((clifd = client[i].fd) < 0)
                continue;
            if (pollfd[i].revents & POLLHUP) {
                /* client has closed conn */
                goto hungup;
            } else if (pollfd[i].revents & POLLIN) {
                /* read arguments buffer from client */
                if ((nread = read(clifd, buf, MAXLINE)) < 0) {
                    log_sys("read error on fd %d", clifd);
                } else if (nread == 0) {
hungup:
                    log_msg("closed: uid %d, fd %d",
                            client[i].uid, clifd);
                    client_del(clifd); /* client has closed conn */
                    pollfd[i].fd = -1;
                    close(clifd);
                } else {
                    request(buf, nread, clifd, client[i].uid);
                }
            }
        }
    }
}

