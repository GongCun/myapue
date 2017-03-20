#include <stdlib.h>
#include <stdio.h>

void *xmalloc(size_t size)
{
    void *p;
    p = malloc(size);
    if (!p) {
        perror("xmalloc");
        exit(EXIT_FAILURE);
    }
    return p;
}
