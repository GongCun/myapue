#include "myapue.h"
#include <time.h>

int main(void)
{
    char buf[MAXLINE];
    time_t tloc;
    struct tm *tmptr;

    if ( time(&tloc) == (time_t)-1 )
        err_sys("time error");
    if ( (tmptr = localtime(&tloc)) == NULL )
        err_sys("localtime error");
    if ( strftime(buf, MAXLINE, "%a %b %d %T %Z %Y", tmptr) != 0 )
        printf("%s\n", buf);
    exit(0);
}
