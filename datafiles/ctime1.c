#include "myapue.h"
#include <time.h>

int main(void)
{
    time_t tloc;

    if (time(&tloc) == (time_t)-1)
        err_sys("time() error");
    printf("%s", ctime(&tloc));
    exit(0);
}
