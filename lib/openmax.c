#include "myapue.h"
#include <errno.h>
#include <unistd.h>
#include <limits.h>

long open_max(void)
{
    long openmax;

    errno = 0;
    if ((openmax = sysconf(_SC_OPEN_MAX)) < 0) {
        if (errno == 0)
            openmax = 256; /* guess */
        else
            err_sys("sysconf error for _SC_OPEN_MAX");
    }
    return(openmax);
}
