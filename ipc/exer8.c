#include "myapue.h"

int main(void)
{
    FILE *fp;
    char line[MAXLINE];

    if ((fp = popen("date 1>&2", "r")) == NULL)
        err_sys("popen error");
    while (fgets(line, MAXLINE, fp) != NULL) {
        if (fputs(line, stdout) == EOF)
            err_sys("fputs error");
    }
    if (ferror(fp))
        err_sys("fgets error");
    if (pclose(fp) < 0)
        err_sys("pclose error");
    exit(0);
}

