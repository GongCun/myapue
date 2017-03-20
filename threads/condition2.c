#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * this simple code demonstrate the use of several Pthread condition variable routines.
 * The main routine creates three threads. Two of the threads work and update a "count"
 * variable. The third thread waits until the count variable reaches a special value.
 */ 

#define NUM_THREADS 3
#define TCOUNT 10
#define COUNT_LIMIT 12

int count = 0;
pthread_mutex_t count_mutex;
pthread_cond_t count_threshold_cv;

void *inc_count(void *t)
{
    int i;
    long my_id = (long)t;

    for (i = 0; i < TCOUNT; i++) {
        pthread_mutex_lock(&count_mutex);
        count++;

        /*
         * check the value of count and signal waiting thread when condition is
         * reached. Note that this occurs while mutex is locked.
         */
        if (count == COUNT_LIMIT) {
            pthread_cond_signal(&count_threshold_cv);
            printf("inc_count(); thread %ld, count = %d, Threshold reached.\n",
                    my_id, count);
        }
        printf("int_count(): thread %ld, count = %d, unlocking mutex.\n",
                my_id, count);
        pthread_mutex_unlock(&count_mutex);

        /* Do some work so that threads can alternate on mutex lock */
        sleep(1);
    }
    pthread_exit(NULL);
}

void *watch_count(void *t)
{
    long my_id = (long)t;

    printf("Starting watch_count(): thread %ld\n", my_id);

    /*
     * lock mutex and wait for signal. Note that the pthead_cont_wait
     * routine will automatically and atomically unlock mutex while it waits.
     * Also, note that if COUNT_LIMIT is reach before this routine is run by 
     * the waiting thread, the loop will be skipped to prevent pthread_cond_wait
     * from never returning
     */
    pthread_mutex_lock(&count_mutex);
    while (count < COUNT_LIMIT) {
        pthread_cond_wait(&count_threshold_cv, &count_mutex);
        printf("watch_count(): thread %ld Condition signal received.\n", my_id);
        count += 125;
        printf("watch_count(): thread %ld count now = %d.\n", my_id, count);
    }
    pthread_mutex_unlock(&count_mutex);
    pthread_exit(NULL);
}

int main(void)
{
    int i;
    long t1 = 1, t2 = 2, t3 = 3;
    pthread_t threads[3];
    pthread_attr_t attr;
    
    /* initialize mutex and condition variable objects */
    pthread_mutex_init(&count_mutex, NULL);
    pthread_cond_init(&count_threshold_cv, NULL);

    /* for portability, explicitly create threads in a joinable state */
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    pthread_create(&threads[0], &attr, watch_count, (void *)t1);
    pthread_create(&threads[1], &attr, inc_count, (void *)t2);
    pthread_create(&threads[2], &attr, inc_count, (void *)t3);

    /* wait for all threads to complete */
    for (i = 0; i < NUM_THREADS; i++)
        pthread_join(threads[i], NULL);
    printf("Main(): Waited on %d threads. Done.\n", NUM_THREADS);

    /* clean up and exit */
    pthread_attr_destroy(&attr);
    pthread_mutex_destroy(&count_mutex);
    pthread_cond_destroy(&count_threshold_cv);
    pthread_exit(NULL);
}
