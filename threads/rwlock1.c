#include <stdlib.h>
#include <pthread.h>

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

/*
 * initialize a queue
 */
int queue_init(struct queue *qp)
{
    int err;
    
    q -> q_head = NULL;
    q -> q_tail = NULL;
    err = pthread_rwlock_init(&q -> q_lock, NULL);
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
    jp -> j_prec = NULL;
    if (qp -> q_head != NULL)
        qp -> q_head -> prev = jp;
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
    pthread_rwlock_wrlock(&qp -> lock);
    if (jp == qp -> q_head) {
        qp -> head = jp -> j_next;
        if (qp -> q_tail == qp)
            qp -> q_tail = NULL;
    } else if (jp == qp -> q_tail) {
        qp -> q_tail = jp -> j_prev;
        if (qp -> q_head == jp)
            qp -> q_head = NULL;
    } else {
        jp -> j_prev -> j_next = jp -> j_next;
        jp -> j_next -> j_prev = jp -> j_prev;
    }
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

