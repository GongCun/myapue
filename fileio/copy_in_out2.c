#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include "myapue.h"


int main(int argc, char **argv)
{
    int n;
    int bufsiz;
    char *buf;

    if (argc < 2) {
        printf("missing argument\n");
        exit(1);
    }
    bufsiz = atol(argv[1]);
    if (errno != 0) {
        perror("atol");
        exit(1);
    }
    buf = (char *)malloc(sizeof(bufsiz));
    if (buf == NULL)
        exit(1);

    while ((n = read(STDIN_FILENO, buf, bufsiz)) > 0)
        if (write(STDOUT_FILENO, buf, n) != n)
            err_sys("write error");

    if (n < 0)
        err_sys("read error");

    return 0;
}
