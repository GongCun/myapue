#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include "myapue.h"

extern int makethread(void *(*)(void *), void *);

struct to_info {
    void (*to_fn)(void *); /* function */
    void *to_arg; /* argument */
    struct timespec to_wait; /* time to wait */
};

#define SECTONSEC 1000000000 /* seconds to nanoseconds */
#define USECTONSEC 1000 /* microseconds to nanoseconds */
#define TCOUNT 10

void clock_gettime(struct timespec *tsp)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    tsp -> tv_sec = tv.tv_sec;
    tsp -> tv_nsec = tv.tv_usec * USECTONSEC;
}

void *timeout_helper(void *arg)
{
    struct to_info *tip;

    tip = (struct to_info*)arg;
    nanosleep(&tip->to_wait, NULL);
    (*tip->to_fn)(tip->to_arg);
    free(arg);
    return (void *)0;
}

void
timeout(const struct timespec *when, void (*func)(void *), void *arg)
{
    struct timespec now;
    struct to_info *tip;
    int err;

    clock_gettime(&now);
    if ((when -> tv_sec > now.tv_sec) ||
            (when -> tv_sec == now.tv_sec && when -> tv_nsec > now.tv_nsec)) {
        tip = malloc(sizeof(struct to_info));
        if (tip != NULL) {
            tip -> to_fn = func;
            tip -> to_arg = arg;
            tip -> to_wait.tv_sec = when -> tv_sec - now.tv_sec;
            if (when -> tv_nsec >= now.tv_nsec) {
                tip -> to_wait.tv_nsec = when -> tv_nsec - now.tv_nsec;
            } else {
                tip -> to_wait.tv_sec--;
                tip -> to_wait.tv_nsec = SECTONSEC - now.tv_nsec + when->tv_nsec;
            }
            err = makethread(timeout_helper, (void *)tip);
            if (err == 0)
                return;
            else
                free(tip);
        }
    }

    /*
     * we get here if (a) when <= now, or (b) malloc fails, or
     * (c) we can't make a thread, so we just call the function now.
     */
    (*func)(arg);
}

pthread_mutexattr_t attr;
pthread_mutex_t mutex;

void retry(void *arg)
{
    pthread_mutex_lock(&mutex);
    /* ... perform retry steps ... */
    printf("arg is %lu\n", (unsigned long)arg);
    pthread_mutex_unlock(&mutex);
}

int main(void)
{
    int err, condition, arg;
    struct timespec when;

    condition = 0;
    arg = 0;
    if ((err = pthread_mutexattr_init(&attr)) != 0)
        err_exit(err, "pthread_mutexattr_init failed");
    if ((err = pthread_mutexattr_settype(&attr,
                    PTHREAD_MUTEX_RECURSIVE)) != 0)
        err_exit(err, "pthread_mutexattr_settype failed");
    if ((err = pthread_mutex_init(&mutex, &attr)) != 0)
        err_exit(err, "can't create recursive mutex");
    /* ... continue processing ... */
    pthread_mutex_lock(&mutex);

    /*
     * check the condition under the protection of a lock to
     * make the check and the call to timeout atomic
     */
    while (condition++ < TCOUNT) {
        /* printf("%d\n", condition); */
        /*
         * calculate the absolute time when we want to retry
         */
        clock_gettime(&when);
        when.tv_sec += 10; /* 10 second from now */
        timeout(&when, retry, (void *)(unsigned long)arg++);
    }
    pthread_mutex_unlock(&mutex);
    /* ... continue processing ... */
    sleep(11);
    return 0;
}

