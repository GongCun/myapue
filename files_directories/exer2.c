#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include "myapue.h"

#define RWXRWXRWX (S_IRWXU|S_IRWXG|S_IRWXO)


int main(void)
{
    umask(0);
    if (creat("foo.exer2", RWXRWXRWX) < 0)
        err_sys("creat error for foo.exer2");
    umask(RWXRWXRWX);
    if (creat("bar.exer2", RWXRWXRWX) < 0)
        err_sys("creat error for bar.exer2");
    exit(0);
}
