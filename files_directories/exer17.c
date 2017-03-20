#include <fcntl.h>
#include "myapue.h"

#define RWXRWXRWX (S_IRWXU|S_IRWXG|S_IRWXO)

int main(void) 
{
    int fd;

    if (unlink("/dev/fd/1") < 0)
        err_sys("unlink error");
    if ((fd = creat("/dev/fd/1", RWXRWXRWX)) < 0)
        err_sys("creat() error");
    exit(0);
}
