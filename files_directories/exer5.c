#include <sys/types.h>
#include <sys/stat.h>
#include "myapue.h"

int main(int argc, char **argv)
{
    struct stat statbuf;
    int i;

    for (i = 1; i < argc; i++) {
        printf("%s: ", argv[i]);
        if (lstat(argv[i], &statbuf) < 0) {
            err_ret("stat error");
            continue;
        }
        printf("st_size = %ld\n", (long)statbuf.st_size);
    }
    exit(0);
}
