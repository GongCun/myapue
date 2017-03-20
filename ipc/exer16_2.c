#include "myapue.h"
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/wait.h>

#define NLOOPS 1000
#define SIZE sizeof(long) /* size of shared memory area */
extern int semcreate(int);
extern void semrmid(int), semgetvalues(int);
extern void semsetvalues(int, unsigned short *, int);

int semid1, semid2, semid3; /* parent, child, mutex */
struct sembuf my_wait, my_post;

static int update(long *ptr)
{
    return (*ptr)++;
}

int main(void)
{
    int i, counter;
    pid_t pid;
    void *area;
    unsigned short val1, val2, val3;

    if (setvbuf(stdout, NULL, _IOLBF, 0) == EOF)
        err_sys("setvbuf error");

    if ((area = mmap(0, SIZE, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANON,
                    -1, 0)) == MAP_FAILED)
        err_sys("mmap error");

    semid1 = semcreate(1);
    semid2 = semcreate(1);
    semid3 = semcreate(1);
    val1 = 1; /* parent semaphore */
    val2 = 0; /* child  semaphore */
    val3 = 1; /* mutex */
    semsetvalues(semid1, &val1, 1);
    semsetvalues(semid2, &val2, 1);
    semsetvalues(semid3, &val3, 1);

    my_wait.sem_num = 0;
    my_wait.sem_op = -1;
    my_wait.sem_flg = SEM_UNDO;
    my_post.sem_num = 0;
    my_post.sem_op = 1;
    my_post.sem_flg = SEM_UNDO;

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid > 0) { /* parent */
        for (i = 0; i < NLOOPS; i += 2) {
            semop(semid1, &my_wait, 1);
            semop(semid3, &my_wait, 1);
            counter = update((long *)area);
            printf("parent: i = %d; area = %ld; counter = %d\n", 
                    i, *((long *)area), counter);
            semop(semid3, &my_post, 1);
            semop(semid2, &my_post, 1);
        } 
    } else { /* child */
        for (i = 1; i < NLOOPS + 1; i += 2) {
            semop(semid2, &my_wait, 1);
            semop(semid3, &my_wait, 1);
            counter = update((long *)area);
            printf("child: i = %d; area = %ld; counter = %d\n", 
                    i, *((long *)area), counter);
            semop(semid3, &my_post, 1);
            semop(semid1, &my_post, 1);
        }
        exit(0);
    }
    /* parent process continue... */
    if (waitpid(pid, NULL, 0) < 0)
        err_sys("waitpid error");
    semrmid(semid1);
    semrmid(semid2);
    semrmid(semid3);
    exit(0);
}
