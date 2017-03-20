#include <limits.h>
#include <sys/stat.h>
#include "myapue.h"

#define RWXRWXRWX (S_IRWXU|S_IRWXG|S_IRWXO)

int main(void)
{
    umask(022);
    int i;

    i = 0;
    while (1) {
        printf("%d\n", i++);
        if (mkdir("abcdefghijklmn", RWXRWXRWX) < 0)
            err_sys("mkdir error");
        if (chdir("abcdefghijklmn") < 0)
            err_sys("chdir error");
    }
    exit(0);
}

