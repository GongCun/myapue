#include "open.h"
#include <fcntl.h>

void request(char *buf, int nread, int fd, uid_t uid)
{
    int newfd;

    if (buf[nread-1] != 0) {
        sprintf(errmsg, "request not null terminated: %*.*s\n",
                nread, nread, buf);
        send_err(fd, -1, errmsg);
        return;
    }

    log_msg("request: %s, from uid %d", buf, uid);

    if (buf_args(buf, cli_args) < 0) { /* parse args & set options */
        send_err(fd, -1, errmsg);
        log_msg(errmsg);
        return;
    }

    if ((newfd = open(pathname, oflag)) < 0) {
        sprintf(errmsg, "can't open %s: %s\n", pathname, 
                strerror(errno));
        send_err(fd, -1, errmsg);
        log_msg(errmsg);
        return;
    }

    if (send_fd(fd, newfd) < 0) /* send the descriptor */
        log_sys("send_fd error");
    log_msg("sent fd %d over fd %d for %s", newfd, fd, pathname);
    close(newfd); /* we're done with descriptor */
}
