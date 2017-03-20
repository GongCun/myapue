#include "myapue.h"
#include <limits.h>
#include <errno.h>

char *
px_ipc_name(const char *name)
{
    char *dir, *dst;
    long pathmax; 
    errno = 0;
    if ((pathmax = pathconf(".", _PC_PATH_MAX)) < 0 && errno == 0) {
        pathmax = 1024; /* guesss */
    } else {
        pathmax++;
    }
    if ((dst = malloc(pathmax)) == NULL)
        return(NULL);
    dir = ".";
    snprintf(dst, pathmax, "%s/%s", dir, name);
    return(dst);
}

char *
Px_ipc_name(const char *name)
{
    char *ptr;

    if ((ptr = px_ipc_name(name)) == NULL)
        err_sys("px_ipc_name error for %s", name);
    return(ptr);
}
