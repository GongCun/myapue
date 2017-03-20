#include "myapue.h"
#include <inttypes.h>

int main(void)
{
    uint32_t i;
    unsigned char *cp;

    i = 0x01020304;
    cp = (unsigned char *)&i;
    printf("%d %d %d %d\n", cp[0], cp[1], cp[2], cp[3]);
    exit(0);
}

