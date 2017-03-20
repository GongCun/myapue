#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "myapue.h"

#define TCOUNT 20

struct foo {
    int f_count;
    pthread_mutex_t f_lock;
    /* ... more stuff here ... */
};

struct foo *fp;

struct foo *
foo_alloc(void) /* allocate the object */
{
    struct foo *fp;
    if ((fp = malloc(sizeof(struct foo))) != NULL) {
        fp->f_count = 1;
        if (pthread_mutex_init(&fp->f_lock, NULL) != 0) /* initialization fail */
        {
            free(fp);
            return NULL;
        }
        /* ... continue initialization ... */
    }
    return fp;
}

void foo_hold(const char *str, struct foo *fp) /* add a reference to the object */
{
    pthread_mutex_lock(&fp->f_lock);
    printf("%s %d\n", str, fp->f_count++);
    pthread_mutex_unlock(&fp->f_lock);
}

void foo_rele(const char *str, struct foo *fp) /* release a reference to the object */
{
    int i;
    for (i = 0; i < TCOUNT; i++) {
        pthread_mutex_lock(&fp->f_lock);
        if (fp -> f_count == 0) {
            printf("%s %d\n", str, fp->f_count);
            pthread_mutex_unlock(&fp->f_lock);
            pthread_exit((void *)0);
        }
        printf("%s %d\n", str, --fp->f_count);
        pthread_mutex_unlock(&fp->f_lock);
    }
    pthread_exit((void *)0);
}

void *thr_fn1(void *arg)
{
    int offset;
    int i;
    
    offset = (long)arg;
    for (i = 0; i < offset; i++)
        foo_hold("thread 1", fp);
    pthread_exit((void *)0);
}

void *thr_fn2(void *arg)
{
    int offset;
    int i;

    offset = (long)arg;
    for (i = 0; i < offset; i++)
        foo_hold("thread 2", fp);
    pthread_exit((void *)0);
}

void *thr_fn3(void *arg)
{
    foo_rele("thread 3", fp);
    pthread_exit((void *)0);
}

void *thr_fn4(void *arg)
{
    foo_rele("thread 4", fp);
    pthread_exit((void *)0);
}

int main(void)
{
    pthread_t tid1, tid2;
    pthread_t tid3, tid4;
    int err;
    void *status;

    if ((fp = foo_alloc()) != NULL) {
        err = pthread_create(&tid1, NULL, thr_fn1, (void *)10);
        if (err != 0)
            err_exit(err, "can't create thread 1");

        err = pthread_create(&tid2, NULL, thr_fn2, (void *)10);
        if (err != 0)
            err_exit(err, "can't create thread 2");

        err = pthread_join(tid1, &status);
        if (err != 0)
            err_exit(err, "can't join with thread 1");
        printf("thread 1 exit code %ld\n", (long)status);

        err = pthread_join(tid2, &status);
        if (err != 0)
            err_exit(err, "can't join with thread 2");
        printf("thread 2 exit code %ld\n", (long)status);

        err = pthread_create(&tid3, NULL, thr_fn3, NULL);
        if (err != 0)
            err_exit(err, "can't create thread 3");

        err = pthread_create(&tid4, NULL, thr_fn4, NULL);
        if (err != 0)
            err_exit(err, "can't create thread 4");

        err = pthread_join(tid3, &status);
        if (err != 0)
            err_exit(err, "can't join with thread 3");
        printf("thread 3 exit code %ld\n", (long)status);

        err = pthread_join(tid4, &status);
        if (err != 0)
            err_exit(err, "can't join with thread 4");
        printf("thread 4 exit code %ld\n", (long)status);

        if (fp->f_count == 0) {
            pthread_mutex_destroy(&fp->f_lock);
            free(fp);
            printf("released\n");
        }
    }

    return 0;
}
