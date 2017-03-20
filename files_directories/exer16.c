#include <limits.h>
#include <sys/stat.h>
#include "myapue.h"

#define RWXRWXRWX (S_IRWXU|S_IRWXG|S_IRWXO)

int main(void)
{
    umask(022);
    char buf[32768];
    while (getcwd(buf, 32768) != NULL) {
        if(strlen(buf) > PATH_MAX) {
            printf("%s\n", buf);
            break; 
        }
        if (mkdir("abcdefghij", RWXRWXRWX) < 0)
            err_sys("mkdir error");
        if (chdir("abcdefghij") < 0)
            err_sys("chdir error");
    }
    exit(0);
}

