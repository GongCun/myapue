#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define TCOUNT 2
#define NUM_THREADS 3

struct job {
    struct job *j_next;
    struct job *j_prev;
    pthread_t j_id; /* tell which thread handles this jobs */
    /* ... more stuff here ... */
};

struct queue {
    struct job *q_head;
    struct job *q_tail;
    pthread_rwlock_t q_lock;
};

struct queue *qp;

/*
 * initialize a queue
 */
int queue_init(struct queue *qp)
{
    int err;
    
    qp -> q_head = NULL;
    qp -> q_tail = NULL;
    err = pthread_rwlock_init(&qp -> q_lock, NULL);
    if (err != 0)
        return(err);
    /* ... continue initialization ... */
    return 0;
}

/*
 * insert a job at the head of the queue
 */
void job_insert(struct queue *qp, struct job *jp)
{
    pthread_rwlock_wrlock(&qp -> q_lock);
    jp -> j_next = qp -> q_head;
    jp -> j_prev = NULL;
    if (qp -> q_head != NULL)
        qp -> q_head -> j_prev = jp;
    else
        qp -> q_tail = jp; /* list was empty */
    qp -> q_head = jp;
    pthread_rwlock_unlock(&qp -> q_lock);
}

/*
 * append a job on the tail of the queue
 */
void job_append(struct queue *qp, struct job *jp)
{
    pthread_rwlock_wrlock(&qp -> q_lock);
    jp -> j_next = NULL;
    jp -> j_prev = qp -> q_tail;
    if (qp -> q_tail != NULL)
        qp -> q_tail -> j_next = jp;
    else
        qp -> q_head = jp; /* list was empty */
    qp -> q_tail = jp;
    pthread_rwlock_unlock(&qp -> q_lock);
}

/*
 * remove the given job from a queue
 */
void job_remove(struct queue *qp, struct job *jp)
{
    pthread_rwlock_wrlock(&qp -> q_lock);
    if (jp == qp -> q_head) {
        qp -> q_head = jp -> j_next;
        if (qp -> q_tail == jp)
            qp -> q_tail = NULL;
    } else if (jp == qp -> q_tail) {
        qp -> q_tail = jp -> j_prev;
        if (qp -> q_head == jp)
            qp -> q_head = NULL;
    } else {
        jp -> j_prev -> j_next = jp -> j_next;
        jp -> j_next -> j_prev = jp -> j_prev;
    }
    free(jp);
    pthread_rwlock_unlock(&qp -> q_lock);
}

/*
 * find a job for the given thread ID
 */
struct job *
job_find(struct queue *qp, pthread_t id)
{
    struct job *jp;
    if (pthread_rwlock_rdlock(&qp -> q_lock) != 0)
        return NULL;

    for (jp = qp -> q_head; jp != NULL; jp = jp -> j_next)
        if (pthread_equal(jp -> j_id, id))
            break;
    pthread_rwlock_unlock(&qp -> q_lock);
    return jp;
}

void *work(void *arg)
{
    int i;
    struct job *jp;
    pthread_t tid;
    
    tid = pthread_self();
    for (i = 0; i < TCOUNT; i++) {
        jp = malloc(sizeof(struct job));
        if (jp != NULL) {
            jp -> j_id = tid;
            job_insert(qp, jp);
        }
        jp = malloc(sizeof(struct job));
        if (jp != NULL) {
            jp -> j_id = tid;
            job_append(qp, jp);
        }
    }

    while ((jp = job_find(qp, tid)) != NULL) {
        /* do some "work" */
        printf("%s\n", (char *)arg);
        /* sleep(1); */
        job_remove(qp, jp);
    }
    pthread_exit((void *)0);
}

int main(void)
{
    pthread_t threads[3];
    pthread_attr_t attr;

    /* initialize mutex */
    qp = malloc(sizeof(struct queue));
    if (qp == NULL)
        return 1;
    queue_init(qp);

    /* for portability, explicitly create threads in a joinable state */
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    pthread_create(&threads[0], &attr, work, "threads 1 work");
    pthread_join(threads[0], NULL);
    pthread_create(&threads[1], &attr, work, "threads 2 work");
    pthread_join(threads[1], NULL);
    pthread_create(&threads[2], &attr, work, "threads 3 work");
    pthread_join(threads[2], NULL);


    /* clean up and exit */
    pthread_attr_destroy(&attr);
    pthread_rwlock_destroy(&qp -> q_lock);
    free(qp);
    printf("Main() Threads Done\n");
    return 0;
}
