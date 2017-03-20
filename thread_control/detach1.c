#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int makethread(void *(*fn)(void *), void *arg)
{
    int err;
    pthread_t tid;
    pthread_attr_t attr;

    err = pthread_attr_init(&attr);
    if (err != 0)
        return(err);

    err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    if (err == 0)
        err = pthread_create(&tid, &attr, fn, arg);
    pthread_attr_destroy(&attr);
    return err;
}

void *fn(void *arg);

int main(void)
{
    makethread(fn, "thread executing");
    return 0;
}

void *fn(void *arg)
{
    printf("%s at address %lx\n", (const char *)arg, (unsigned long)pthread_self());
    return (void *)0;
}
