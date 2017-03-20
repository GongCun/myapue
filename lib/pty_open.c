#include "myapue.h"
#include <errno.h>
#include <fcntl.h>

int ptym_open(char *pts_name, int pts_namesz)
{
    char *ptr;
    int fdm, err;

    if ((fdm = posix_openpt(O_RDWR)) < 0)
        return(-1);
    if (grantpt(fdm) < 0) /* grant access to slave */
        goto errout;
    if (unlockpt(fdm) < 0) /* clear slave's lock flag */
        goto errout;
    if ((ptr = ptsname(fdm)) == NULL) /* get slave's name */
        goto errout;

    /*
     * return name of slave, null terminate to handle
     * case where strlen(ptr) > pts_namesz
     */
    strncpy(pts_name, ptr, pts_namesz);
    pts_name[pts_namesz-1] = '\0';
    return(fdm); /* return fd of master */
errout:
    err = errno;
    close(fdm);
    errno = err;
    return(-1);
}

int ptys_open(char *pts_name)
{
    int fds;

    if ((fds = open(pts_name, O_RDWR)) < 0)
        return(-1);
    return(fds);
}
