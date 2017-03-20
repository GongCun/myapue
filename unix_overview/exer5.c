#include "myapue.h"
#include <time.h>

int main(void)
{
    time_t time = (time_t)2147483647;
    printf("%s", ctime(&time));


    /* the correct method */
    time = 0x7FFFFFFF;
    printf("%s", asctime(gmtime(&time)));

    exit(0);
}
