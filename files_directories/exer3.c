#include <sys/types.h>
#include <sys/stat.h>
#include "myapue.h"

int main(void)
{
    struct stat statbuf;

    if (stat("read.exer3", &statbuf) < 0)
        err_sys("stat error");
    if (chmod("read.exer3", statbuf.st_mode & ~S_IRUSR) < 0)
        err_sys("chmod error");
    if (open("read.exer3", O_RDONLY) < 0)
        err_ret("open error");
    exit(0);
}
