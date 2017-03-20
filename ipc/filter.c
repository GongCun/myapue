#include "myapue.h"
#include <sys/wait.h>

int main(void)
{
    char line[MAXLINE];
    FILE *fp;

    if ((fp = popen("./myuclc", "r")) == NULL)
        err_sys("popen error");
    for ( ; ; ) {
        fputs("prompt> ", stdout);
        fflush(stdout);
        if (fgets(line, MAXLINE, fp) == NULL)
            break;
        if (fputs(line, stdout) == EOF)
            err_sys("fputs error");
    }
    if (ferror(fp))
        err_sys("fgets error");
    if (pclose(fp) < 0)
        err_sys("pclose error");
    putchar('\n');
    exit(0);
}

