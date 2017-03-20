#include "myapue.h"
#include <time.h>

int main(void)
{
    time_t tloc;

    tloc = (time_t)2147483647; /* 32 bit long int */
    printf("%s", ctime(&tloc));
    tloc = (time_t)4294967295; /* 32 bit unsigned long */ 
    printf("%s", ctime(&tloc));
    tloc = (time_t)8589934592;
    printf("%s", ctime(&tloc));
    tloc = (time_t)17179869184;
    printf("%s", ctime(&tloc));
    tloc = (time_t)34359738368;
    printf("%s", ctime(&tloc));
    tloc = (time_t)68719476736;
    printf("%s", ctime(&tloc));
    tloc = (time_t)137438953472;
    printf("%s", ctime(&tloc));
    tloc = (time_t)274877906944;
    printf("%s", ctime(&tloc));
    tloc = (time_t)549755813888;
    printf("%s", ctime(&tloc));
    exit(0);
}
