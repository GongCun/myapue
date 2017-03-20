#include "myapue.h"

/* 
 * read a text line from a descriptor, one byte at a time
 * (example only)
 */

ssize_t readline(int fd, void *vptr, size_t maxline)
{
    ssize_t n, rc;
    char c, *ptr;

    ptr = vptr;
    for (n = 1; n < maxline; n++) {
again:
        if ((rc = read(fd, &c, 1)) == 1) {
            *ptr++ = c;
            if (c == '\n')
                break; /* newline is store, like fgets() */
        } else if (rc == 0) {
            *ptr = 0;
            return(n - 1); /* EOF, n-1 bytes were read */
        } else {
            if (errno == EINTR)
                goto again;
            return(-1); /* error, errno set by read() */
        }
    }
    *ptr = 0;
    return(n);
}

