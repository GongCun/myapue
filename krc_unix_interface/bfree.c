#include "syscalls.h"

unsigned int bfree(char *p, unsigned int n) /* n bytes */
{
    Header *hp;

    if (n < sizeof(Header))
        return 0; /* too small to be useful */

    hp = (Header *)p;
    hp->s.size = n/sizeof(Header);
    krfree((void *)(hp+1));
    return hp->s.size;
}


