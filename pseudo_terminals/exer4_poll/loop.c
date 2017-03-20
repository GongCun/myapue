#include "myapue.h"
#include <sys/time.h>
#include <poll.h>


#define BUFFSIZE 512

void loop(int ptym, int ignoreeof)
{
    int nread, nfds, ignore = 0;
    char buf[BUFFSIZE];
    struct pollfd fds[2];
    fds[0].fd = STDIN_FILENO;
    fds[0].events = POLLIN;
    fds[1].fd = ptym;
    fds[1].events = POLLIN;
    nfds = 2;

    for (;;) {
        if (poll(fds, nfds, -1) < 0) {
            if (errno == EINTR)
                continue;
            else 
                err_sys("poll error");
        }

        if (fds[0].revents & POLLIN) {
            if ((nread = read(STDIN_FILENO, buf, BUFFSIZE)) < 0)
                err_sys("read error from stdin");
            else if (nread == 0) {
                if (ignoreeof == 0) { 
                    break; /* EOF on stdin means we're done */
                } else {
                    fds[0].fd = -1;
                    close(STDIN_FILENO);
                }
            } else { 
                if (writen(ptym, buf, nread) != nread) 
                    err_sys("written error on master pty");
            }
        }

        if (fds[1].revents & POLLIN) {
            if ((nread = read(ptym, buf, BUFFSIZE)) <= 0) 
                break;  /* error, or EOF */ 
            if (writen(STDOUT_FILENO, buf, nread) != nread) 
                err_sys("writen error to stdout");
        }
    }
    exit(0);
}

