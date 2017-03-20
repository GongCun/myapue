#ifndef _SYSCALLS_H
#define _SYSCALLS_H

#include "myapue.h"

#define EOF (-1)
#define BUFSIZ 1024
#define OPEN_MAX 20

typedef struct _iobuf {
    int cnt; /* character left */
    char *ptr; /* next character position */
    char *base; /* location of buffer */
    int flag; /* mode of file access */
    int fd; /* file descriptor */
} KRFILE;

typedef long Align; /* for alignment to long boundary */

union header { /* block header */
    struct {
        union header *ptr; /* next block if on free list */
        unsigned int size;
    } s;
    Align x;
};

typedef union header Header;

extern KRFILE _iob[OPEN_MAX];

#define krstdin (&_iob[0])
#define krstdout (&_iob[1])
#define krstderr (&_iob[2])

enum _flags {
    _READ = 01, /* file open for reading */
    _WRITE = 02, /* file open for writing */
    _UNBUF = 04, /* file is unbuffered */
    _EOF = 010, /* EOF has occurred on this file */
    _ERR = 020 /* error occurred on this file */
};

int _fillbuf(KRFILE *);
int _flushbuf(int, KRFILE *);
KRFILE *krfopen(char *, char *);
int krfclose(KRFILE *);
int xfflush(KRFILE *);
int krfseek(KRFILE *, off_t, int);

#define krgetc(p) (--(p)->cnt >= 0 \
        ? (unsigned char) *(p)->ptr++ : _fillbuf(p))

#define krputc(x,p) (--(p)->cnt >= 0 \
        ? *(p)->ptr++ = (x) : _flushbuf((x), p))

/* krmalloc: general-purpose storage allocator */
void *krmalloc(unsigned int);
void krfree(void *);
void *krcalloc(unsigned int, unsigned int);
void *krmalloc_safe(unsigned int);
void krfree_safe(void *);
unsigned int bfree(char *, unsigned int);

#endif
