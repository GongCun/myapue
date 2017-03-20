#include "myapue.h"

int main(void)
{
    char buf[MAXLINE];

    while (fgets(buf, MAXLINE, stdin) != NULL) {
        if (fputs(buf, stdout) == EOF)
            err_sys("output error");
        if (fflush(stdout) == EOF)
            err_sys("fflush error");
        if ( fsync(fileno(stdout)) < 0 )
            err_sys("fsync error");
    }
        
    if (ferror(stdin))
        err_sys("input error");

    exit(0);
}
