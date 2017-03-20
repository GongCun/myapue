#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "myapue.h"

struct foo {
    int a, b, c, d;
};

void printfoo(const char *s, const struct foo *fp)
{
    printf("%s", s);
    printf(" structrue at 0x%lx\n", (unsigned long)fp);
    printf(" foo.a = %d\n", fp->a);
    printf(" foo.b = %d\n", fp->b);
    printf(" foo.c = %d\n", fp->c);
    printf(" foo.d = %d\n", fp->d);
}

void *
thr_fn1(void *arg)
{
    printfoo("thread 1:\n", (struct foo *)arg);
    pthread_exit(arg);
}

int main(void)
{
    int err;
    pthread_t tid1;
    struct foo foo = {1, 2, 3, 4};
    void *tret;

    err = pthread_create(&tid1, NULL, thr_fn1, (void *)&foo);
    if (err != 0)
        err_exit(err, "can't create thread 1");
    err = pthread_join(tid1, &tret);
    if (err != 0)
        err_exit(err, "can't join with thread 1");
    printfoo("parent:\n",(struct foo *)tret);
    return 0;
}
