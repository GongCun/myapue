#include "syscalls.h"

int xfflush(KRFILE *fp)
{
    int rc = 0;

    if (fp < _iob || fp >= _iob+OPEN_MAX)
        return EOF;
    if (fp->flag & _WRITE)
        rc = _flushbuf(0, fp);
    fp->ptr = fp->base;
    fp->cnt = (fp->flag & _UNBUF) ? 1 : BUFSIZ;
    return rc;
}
