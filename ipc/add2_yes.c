#include "myapue.h"

int main(void)
{
    int int1, int2;
    char line[MAXLINE];

    if (setvbuf(stdin, NULL, _IOLBF, 0) != 0)
        err_sys("setvbuf error");
    if (setvbuf(stdout, NULL, _IOLBF, 0) != 0)
        err_sys("setvbuf error");
    while (fgets(line, MAXLINE, stdin) != NULL) {
        if (sscanf(line, "%d%d", &int1, &int2) == 2) {
            if (printf("%d\n", int1 + int2) == EOF)
                err_sys("printf error");
        } else {
            if (fputs("invalid args\n", stdout) == EOF)
                err_sys("fputs error");
        }
    }
    if (ferror(stdin))
        err_sys("fgets error");
    exit(0);
}

