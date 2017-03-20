#include "myapue.h"
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/wait.h>

#define NLOOPS 10000000
#define SIZE sizeof(long) /* size of shared memory area */
extern void my_lock(void), my_unlock(void), my_dtlock();

static int update(long *ptr)
{
    return (*ptr)++;
}

int main(void)
{
    int i, counter;
    pid_t pid;
    void *area;

    if (setvbuf(stdout, NULL, _IOLBF, 0) == EOF)
        err_sys("setvbuf error");

    if ((area = mmap(0, SIZE, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANON,
                    -1, 0)) == MAP_FAILED)
        err_sys("mmap error");

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid > 0) { /* parent */
        for (i = 1; i < NLOOPS + 1; i += 2) {
            my_lock();
            counter = update((long *)area);
            printf("parent: i = %d; area = %ld; counter = %d\n", 
                    i, *((long *)area), counter);
            my_unlock();
        } 
    } else { /* child */
        for (i = 0; i < NLOOPS; i += 2) {
            my_lock();
            counter = update((long *)area);
            printf("child: i = %d; area = %ld; counter = %d\n", 
                    i, *((long *)area), counter);
            my_unlock();
        }
        exit(0);
    }
    /* parent process continue... */
    if (waitpid(pid, NULL, 0) < 0)
        err_sys("waitpid error");
    my_dtlock();
    exit(0);
}
