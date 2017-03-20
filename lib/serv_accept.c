#include "myapue.h"
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <time.h>
#include <errno.h>

#define STALE 30 /* client's name can't be older than this (sec) */

/*
 * wait for a client connection to arrive, and accept it
 * we also obtain the client's ID from the pathname
 * that it must bind before calling us.
 * returns new fd if all OK, <0 error
 */
int serv_accept(int listenfd, uid_t *uidptr)
{
    int clifd, err, rval;
    socklen_t len;
    time_t staletime;
    struct sockaddr_un un;
    struct stat statbuf;

    len = sizeof(un);
    if ((clifd = accept(listenfd, (struct sockaddr *)&un, &len)) < 0)
        return(-1); /* often errno == EINTR, if signal caught */

    /* obtain the client's uid from its calling address */
    len -= offsetof(struct sockaddr_un, sun_path); /* len of pathname */
    un.sun_path[len] = 0; /* null terminate */

    if (stat(un.sun_path, &statbuf) < 0) {
        rval = -2;
        goto errout;
    }
    if (S_ISSOCK(statbuf.st_mode) == 0) {
        rval = -3; /* not a socket */
        goto errout;
    }
    if ((statbuf.st_mode & S_IRWXU) != S_IRWXU) {
        rval = -4; /* is not rwx****** */
        goto errout;
    }
    staletime = time(NULL) - STALE;
    if (statbuf.st_atime < staletime ||
            statbuf.st_ctime < staletime ||
            statbuf.st_mtime < staletime) {
        rval = -5; /* i-node is too old */
        goto errout;
    }
    if (uidptr != NULL)
        *uidptr = statbuf.st_uid; /* return uid of caller */
    unlink(un.sun_path);
    return(clifd);
errout:
    err = errno;
    close(clifd);
    errno = err;
    return(rval);
}

int Serv_accept(int listenfd, uid_t *uidptr)
{
    return(serv_accept(listenfd, uidptr));
}
