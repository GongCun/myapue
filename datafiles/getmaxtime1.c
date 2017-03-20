#include "myapue.h"
#include <time.h>

int main(void)
{
    struct tm *tmptr;
    time_t tloc;
    /* long maxtime = 9223372036854775806; */
    long maxtime = 67768036191676799;
    char buf[MAXLINE];

    /* printf("%zd\n", sizeof(tloc)); */
    tloc = (time_t)maxtime;

    if ( (tmptr = localtime(&tloc)) == NULL )
        err_sys("localtime() error");

    if ( strftime(buf, MAXLINE, "%a %b %d %T %Z %Y", tmptr) != 0 )
        printf("%s\n", buf);

    exit(0);
}
