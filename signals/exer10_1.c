#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "myapue.h"

int main(void)
{
    time_t tp;
    struct tm *lt;

    if (time(&tp) < 0)
        err_sys("time() error");
    lt = localtime(&tp);

    printf("%d %d %d %d %d %d\n", 
            lt->tm_year + 1900,
            lt->tm_mon + 1,
            lt->tm_mday,
            lt->tm_hour,
            lt->tm_min,
            lt->tm_sec);

    return 0;
}

    
