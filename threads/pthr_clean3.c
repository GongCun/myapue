#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "myapue.h"

void cleanup(void *arg)
{
    printf("cleanup: %s\n", (char *)arg);
}

void *
thr_fn1(void *arg)
{
    /*
     * setup enabel cancel thread and deferred type
     */
    int unused;
    int ret;

    ret = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &unused); /* default */
    if (ret) {
        errno = ret;
        perror("pthread_setcancelstate");
        return (void *)-1;
    }

    ret = pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, &unused); /* default */
    if (ret) {
        errno = ret;
        perror("pthread_setcanceltype");
        return (void *)-1;
    }

    printf("thread 1 start\n");
    pthread_cleanup_push(cleanup, "thread 1 first handler");
    pthread_cleanup_push(cleanup, "thread 1 second handler");
    printf("thread 1 push complete\n");
    if (arg)
        return (void *) 1;
    pthread_cleanup_pop(0);
    pthread_cleanup_pop(0);
    return (void *) 1;
}

void *
thr_fn2(void *arg)
{
    printf("thread 2 start\n");
    pthread_cleanup_push(cleanup, "thread 2 first handler");
    pthread_cleanup_push(cleanup, "thread 2 second handler");
    printf("thread 2 push complete\n");
    if (arg)
        pthread_exit((void *)2);
    pthread_cleanup_pop(0);
    pthread_cleanup_pop(0);
    pthread_exit((void *)2);
}

int main(void)
{
    int err;
    pthread_t tid1, tid2;
    void *tret;

    err = pthread_create(&tid1, NULL, thr_fn1, (void *)1);
    if (err != 0)
        err_exit(err, "can't create thread 1");

    err = pthread_create(&tid2, NULL, thr_fn2, (void *)1);
    if (err != 0)
        err_exit(err, "can't create thread 2");

    /* cancel thread 1 */
    err = pthread_cancel(tid1);
    if (err != 0)
        err_exit(err, "can't cancel thread 1");

    err = pthread_join(tid2, &tret);
    if (err != 0)
        err_exit(err, "can't join with thread 2");
    printf("thread 2 exit code %ld\n", (long)tret);
    return 0;
}

