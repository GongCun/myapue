#include "syscalls.h"

int _flushbuf(int x, KRFILE *fp)
{
    int bufsize;
    unsigned int nc; /* size of buffer alloc. */

    if ((fp->flag & (_WRITE|_EOF|_ERR)) != _WRITE) 
        return EOF;
    bufsize = (fp->flag & _UNBUF) ? 1 : BUFSIZ; /* BUFSIZ == 1024 */
    if (fp->base == NULL) { /* no buffer yet */
        if ((fp->base = malloc(bufsize)) == NULL) {
            fp->flag |= _ERR;
            return EOF;
        }
    } else { 
        nc = fp->ptr - fp->base;
        if (write(fp->fd, fp->base, nc) != nc) {
            fp->flag |= _ERR;
            return EOF;
        }
    }
    fp->ptr = fp->base;
    fp->cnt = bufsize - 1;

    return (*fp->ptr++ = (unsigned char)x);
}

