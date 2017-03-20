#include "myapue.h"
#include <pthread.h>
#include <semaphore.h>

#define NBUFF 10
#define SEM_MUTEX "mutex"
#define SEM_NEMPTY "nempty"
#define SEM_NSTORED "nstored"

int nitems; /* read-only by producer and consumer */
struct { /* data shared by producer and consumer */
    int buff[NBUFF]; 
    sem_t *mutex, *nempty, *nstored;
} shared;

void *produce(void *), *consume(void *);

int main(int argc, char **argv)
{
    pthread_t tid_produce, tid_consume;

    if (argc != 2)
        err_quit("%s <#items>", *argv);
    nitems = atoi(argv[1]);

    /* create three semaphores */
    shared.mutex = sem_open(Px_ipc_name(SEM_MUTEX), O_CREAT|O_EXCL, FILE_MODE, 1);
    shared.nempty = sem_open(Px_ipc_name(SEM_NEMPTY), O_CREAT|O_EXCL, FILE_MODE, NBUFF);
    shared.nstored = sem_open(Px_ipc_name(SEM_NSTORED), O_CREAT|O_EXCL, FILE_MODE, 0);

    /* create one producer thread and one consumer thread */
    pthread_create(&tid_produce, NULL, produce, NULL);
    pthread_create(&tid_consume, NULL, consume, NULL);

    /* wait for the two threads */
    pthread_join(tid_produce, NULL);
    pthread_join(tid_consume, NULL);

    /* remove the semaphores */
    sem_unlink(Px_ipc_name(SEM_MUTEX));
    sem_unlink(Px_ipc_name(SEM_NEMPTY));
    sem_unlink(Px_ipc_name(SEM_NSTORED));
    exit(0);
}

void *produce (void *arg)
{
    int i;
    for (i = 0; i < nitems; i++) {
        sem_wait(shared.nempty); /* wait for at least 1 empty slot */
        sem_wait(shared.mutex);
        shared.buff[i%NBUFF] = i; /* store i into circular buffer */
        sem_post(shared.mutex);
        sem_post(shared.nstored); /* 1 more stored item */
    }
    return(arg);
}

void *consume (void *arg)
{
    int i;
    for (i = 0; i < nitems; i++) {
        sem_wait(shared.mutex); /* it will cause deadlock */
        sem_wait(shared.nstored); /* wait for at least 1 stored slot */
        if (shared.buff[i%NBUFF] == i )
            printf("buff[%d] = %d\n", i%NBUFF, i);
        sem_post(shared.mutex);
        sem_post(shared.nempty); /* 1 more empty item */
    }
    return(arg);
}

