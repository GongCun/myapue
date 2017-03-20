#include "syscalls.h"

int krfclose(KRFILE *fp)
{
    int rc = 0;

    if (fp < _iob || fp >= _iob+OPEN_MAX)
        return EOF;

    if ((rc = xfflush(fp)) != EOF) {
        free(fp->base);
        fp->ptr = NULL;
        fp->base = NULL;
        fp->cnt = 0;
        fp->flag &= ~(_WRITE|_READ);
    }

    return rc;
}
