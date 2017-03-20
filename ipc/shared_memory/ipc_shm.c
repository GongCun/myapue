#include "myapue.h"
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/shm.h>

#define NLOOPS 1000
#define SIZE sizeof(long) /* size of shared memory area */
#define SHM_MODE 0600

static int update(long *ptr)
{
    return (*ptr)++;
}

int main(void)
{
    int i, counter;
    pid_t pid;
    void *area;
    int shmid;

    if (setvbuf(stdout, NULL, _IOLBF, 0) == EOF)
        err_sys("setvbuf error");

    if ((shmid = shmget(IPC_PRIVATE, SIZE, SHM_MODE)) < 0)
        err_sys("shmget error");
    if ((area = shmat(shmid, 0, 0)) == (void *)-1)
        err_sys("shmat error");

    TELL_WAIT();
    
    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid > 0) { /* parent */
        for (i = 1; i < NLOOPS + 1; i += 2) {
            WAIT_CHILD();
            if ((counter = update((long *)area)) != i)
                err_quit("parent: expected %d, got %d", i, counter);
            printf("parent: i = %d; area = %ld; counter = %d\n", 
                    i, *((long *)area), counter);
            TELL_CHILD(pid);
        } 
    } else { /* child */
        for (i = 0; i < NLOOPS; i += 2) {
            if ((counter = update((long *)area)) != i)
                err_quit("child: expected %d, got %d", i, counter);
            printf("child: i = %d; area = %ld; counter = %d\n", 
                    i, *((long *)area), counter);
            TELL_PARENT(getppid());
            WAIT_PARENT();
        }
    }
        
    exit(0);
}
