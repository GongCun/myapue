#include "syscalls.h"

int krfseek(KRFILE *fp, off_t offset, int whence)
{
    int rc = 0, nc;

    if (fp->flag & _READ) {
        if (whence == SEEK_CUR)
            offset -= fp->cnt; 
        rc = lseek(fp->fd, offset, whence);
        fp->cnt = 0;
    } else if (fp->flag & _WRITE) {
        if ((nc = fp->ptr - fp->base) > 0)
            if (write(fp->fd, fp->base, nc) != nc)
                rc = -1;
        if (rc != -1)
            rc = lseek(fp->fd, offset, whence);
    }
    return (rc == -1) ? -1 : 0;
}
        


