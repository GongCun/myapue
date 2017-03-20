#include "myapue.h"
#include <ctype.h>
#include <fcntl.h>

#define BSZ 4096

unsigned char buf[BSZ];

unsigned char
translate(unsigned char c)
{
    if (isalpha(c)) {
        c = toupper(c);
    }
    return(c);
}

int main(int argc, char **argv)
{
    int ifd, ofd, i, n, nw;
    if (argc != 3)
        err_quit("usage: %s infile outfile", argv[1]);
    if ((ifd = open(argv[1], O_RDONLY)) < 0)
        err_sys("open infile error");
    if ((ofd = open(argv[2], O_RDWR|O_TRUNC|O_CREAT, 
                    FILE_MODE)) < 0)
        err_sys("open outfile error");

    while ((n = read(ifd, buf, BSZ)) > 0) {
        for (i = 0; i < n; i++)
            buf[i] = translate(buf[i]);
        if ((nw = write(ofd, buf, n)) != n) {
            if (nw < 0)
                err_sys("write error");
            else
                err_quit("short write (%d/%d)", nw, n);
        }
    }
    fsync(ofd);
    exit(0);
}
