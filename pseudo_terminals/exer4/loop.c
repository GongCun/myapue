#include "myapue.h"
#include <sys/time.h>
#include <sys/select.h>

#define BUFFSIZE 512

void loop(int ptym, int ignoreeof)
{
    int nread, maxfd, ignore = 0;
    char buf[BUFFSIZE];
    fd_set rset;

    maxfd = STDIN_FILENO > ptym ? STDIN_FILENO : ptym;

    for (;;) {
        FD_ZERO(&rset);
        FD_SET(ptym, &rset);
        if (ignore == 0) {
            FD_SET(STDIN_FILENO, &rset);
        } else {
            maxfd = ptym;
        }

        if (select(maxfd+1, &rset, NULL, NULL, NULL) < 0)
            err_sys("select error");

        if (FD_ISSET(STDIN_FILENO, &rset)) { 
            if ((nread = read(STDIN_FILENO, buf, BUFFSIZE)) < 0)
                err_sys("read error from stdin");
            else if (nread == 0) {
                if (ignoreeof == 0) { 
                    break; /* EOF on stdin means we're done */
                } else {
                    ignore = 1;
                }
            } else { 
                if (writen(ptym, buf, nread) != nread) 
                    err_sys("written error on master pty");
            }
        }

        if (FD_ISSET(ptym, &rset)) { 
            if ((nread = read(ptym, buf, BUFFSIZE)) <= 0) 
                break;  /* error, or EOF */ 
            if (writen(STDOUT_FILENO, buf, nread) != nread) 
                err_sys("writen error to stdout");
        }
    }
    exit(0);
}

