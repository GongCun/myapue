#include "syscalls.h"

void *krcalloc(unsigned int n, unsigned int size)
{
    unsigned int i, nb;
    char *p, *q;

    nb = n * size;

    if ((p = q = malloc(nb)) != NULL)
        for (i = 0; i < nb; i++)
            *p++ = 0;
    return (void *)q;
}
