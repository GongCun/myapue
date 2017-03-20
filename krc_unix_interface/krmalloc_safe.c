#include "syscalls.h"

#define MAXBYTES (unsigned int) 10240

static Header base; /* empty list to get started */
static Header *freep = NULL; /* start of free list */
static Header *morecore(unsigned int);
static unsigned int maxalloc;

void *krmalloc_safe(unsigned int nbytes)
{
    Header *p, *prevp;
    unsigned int nunits;

    if (nbytes > MAXBYTES) { /* not more than MAXBYTES */
        fprintf(stderr, "alloc: can't allocate more the %u bytes\n",
                MAXBYTES);
        return NULL;
    }
    nunits = (nbytes+sizeof(Header)-1)/sizeof(Header) + 1;
    if ((prevp = freep) == NULL) { /* no free list yet */
        base.s.ptr = freep = prevp = &base;
        base.s.size = 0;
    }
    for (p = prevp->s.ptr; ; prevp = p, p = p->s.ptr) {
        if (p->s.size >= nunits) { /* big enough */
            if (p->s.size == nunits) /* exactly */
                prevp->s.ptr = p->s.ptr;
            else {
                p->s.size -= nunits;
                p += p->s.size;
                p->s.size = nunits;
            }
            freep = prevp;
            return (void *)(p+1);
        }
        if (p == freep) /* wrapped around free list */
            if ((p = morecore(nunits)) == NULL)
                return NULL; /* none left */
    }
}

#define NALLOC 1024 /* minimum #units to request */

/* morecore: ask system for more memory */
static Header *morecore(unsigned int nu)
{
    char *cp;
    Header *up;

    if (nu < NALLOC)
        nu = NALLOC;
    cp = sbrk(nu * sizeof(Header));
    if (cp == (char *)-1) /* no space at all */
        return NULL;
    up = (Header *)cp;
    up->s.size = nu;
    maxalloc = (up->s.size > maxalloc) ? up->s.size : maxalloc;
    krfree_safe((void *)(up+1));
    return freep;
}

/* free: put block ap in free list */
void krfree_safe(void *ap)
{
    Header *bp, *p;

    bp = (Header *)ap - 1; /* point to block header */
    if (bp->s.size == 0 || bp->s.size > maxalloc) {
        fprintf(stderr, "free, can't free %u units\n",
                bp->s.size);
        return;
    }

    for (p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
        if (p >= p->s.ptr && (bp > p || bp < p->s.ptr))
            break; /* freed block at start of end of arena */

    if (bp + bp->s.size == p->s.ptr) { /* join to upper nbr */
        bp->s.size += p->s.ptr->s.size;
        bp->s.ptr = p->s.ptr->s.ptr;
    } else
        bp->s.ptr = p->s.ptr;
    if (p + p->s.size == bp) { /* join to lower nbr */
        p->s.size += bp->s.size;
        p->s.ptr = bp->s.ptr;
    } else
        p->s.ptr = bp;

    freep = p;
}
