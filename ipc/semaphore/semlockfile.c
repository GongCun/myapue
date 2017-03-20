#include "myapue.h"
#include <semaphore.h>
#include <pthread.h>

#define LOCK_PATH "pxsemlock"

sem_t *locksem;
int initflag;

void my_lock(int fd)
{
    if (initflag == 0) {
        if ((locksem = sem_open(Px_ipc_name(LOCK_PATH), 
                        O_CREAT, FILE_MODE, 1)) == SEM_FAILED)
            err_sys("sem_open error");
        initflag = 1;
    }
    if (sem_wait(locksem) < 0)
        err_sys("sem_wait error");
}

void my_unlock(void)
{
    if (sem_post(locksem) < 0)
        err_sys("sem_post error");
}

int main(void)
{
    int fd;
    pid_t pid;

    fd = open("./test.file", O_CREAT|O_RDWR, FILE_MODE);
    if (fd < 0)
        err_sys("open error");
    TELL_WAIT();

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) { /* child */
        my_lock(fd);
        printf("child got the lock\n");
        my_unlock();
    } else { /* parent */ 
        my_lock(fd);
        printf("parent get the lock;sleeping\n");
        sleep(10);
        my_unlock();
    }
    exit(0);
}



