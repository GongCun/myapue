#include "myapue.h"

int send_err(int fd, int errcode, const char *msg)
{
    int n;
    if ((n = strlen(msg)) > 0)
        if (writen(fd, (void *)msg, n) != n) /* send the error message */
            return(-1);
    if (errcode >= 0)
        errcode = -1; /* must be negative */
    if (send_fd(fd, errcode) < 0)
        return(-1);
    return(0);
}
