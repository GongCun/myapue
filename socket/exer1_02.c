#include "myapue.h"
#include <inttypes.h>
#include <arpa/inet.h>

int main(void)
{
    uint32_t i, ni;
    unsigned char *cp;

    i = 0x01020304;
    cp = (unsigned char *)&i;
    printf("host: %d %d %d %d\n", cp[0], cp[1], cp[2], cp[3]);

    ni = htonl(i);
    cp = (unsigned char *)&ni;
    printf("network: %d %d %d %d\n", cp[0], cp[1], cp[2], cp[3]);

    exit(0);
}

