#include "myapue.h"
#include <stdio.h>

#define CMD "notexistcommand"

int main(void)
{
    FILE *fp;
    char buf[MAXLINE];
    int rc;

    if ((fp = popen(CMD, "r")) == NULL)
        err_sys("popen error");
    while (fgets(buf, MAXLINE, fp) != NULL) {
        printf("get\n");
        if (fputs(buf, stdout) == EOF)
            err_sys("fputs error to stdout");
    }
    if (ferror(fp))
        err_sys("fgets error");
    if ((rc = pclose(fp)) < 0)
        err_sys("pclose error");
    printf("rc = %d\n", rc);
    exit(0);
}
    
