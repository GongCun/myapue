#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "myapue.h"

struct foo {
    int f_count;
    pthread_mutext_t f_lock;
    int f_id;
    /* ... more stuff here ... */
}

struct foo *
foo_alloc(void) /* allocate the object */
{
    struct foo *fp;
    if ((fp = malloc(sizeof(struct foo))) != NULL) {
        fp->f_count = 1;
        if (pthread_mutex_init(&fp->f_lock, NULL) != 0) /* fail initialization */
        {
            free(fp);
            return NULL;
        }
        /* ... continue initialization ... */
    }
    return fp;
}

void foo_hold(struct foo *fp) /* add a reference to the object */
{
    pthread_mutex_lock(&fp->f_lock);
    fp->count++;
    pthread_mutex_unlock(&fp->f_lock);
}

void foo_rele(struct foo *fp) /* release a reference to the object */
{
    pthread_mutex_lock(&fp->f_lock);
    if (--fp->count == 0) { /* last reference */
        pthread_mutex_unlock(&fp->f_lock);
        pthread_mutex_destroy(&fp->f_lock);
        free(fp);
    } else {
        pthread_mutex_unlock(&fp->f_lock);
    }
}
