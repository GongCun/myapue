#include "myapue.h"
#include <fcntl.h>

#define O_OPTION (O_RDWR|O_CREAT|O_APPEND|O_TRUNC)
#define MODE (S_IRUSR|S_IWUSR)

int main(void)
{
    int i, fd;
    char *buf = "1";

    if ( (fd = open("tempfile", O_OPTION, MODE)) < 0)
        err_sys("open error");
    for (i = 0 ; i < 103316352; i++)
        if (write(fd, buf, 1) != 1) 
            err_sys("write error");
    exit(0);
}
