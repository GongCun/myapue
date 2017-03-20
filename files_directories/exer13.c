#include <time.h>
#include <sys/stat.h>
#include <utime.h>
#include "myapue.h"

int main(void)
{
    time_t tloc;
    struct stat statbuf;
    struct utimbuf timebuf;

    if (stat("tempfile", &statbuf) < 0)
        err_sys("stat error");
    if (time(&tloc) == (time_t)-1)
        err_sys("time() error");
    timebuf.actime = statbuf.st_atime;
    timebuf.modtime = tloc;
    if (utime("tempfile", &timebuf) < 0)
        err_sys("utime error");
    exit(0);
}
