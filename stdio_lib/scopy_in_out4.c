#include "myapue.h"

int main(void)
{
    char buf[4];

    while (fgets(buf, sizeof(buf), stdin) != NULL)
        if (fputs(buf, stdout) == EOF)
            err_sys("output error");

    if (ferror(stdin))
        err_sys("input error");

    exit(0);
}
