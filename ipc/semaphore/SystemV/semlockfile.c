#include "myapue.h"
#include <sys/sem.h>

#define LOCK_PATH "./svsemlock"
#define MAX_TRIES 10

int semid, initflag;
struct sembuf postop, waitop;

void my_lock(int fd)
{
    int oflag, i;
    union semun arg;
    struct semid_ds seminfo;

    if (initflag == 0) {
        oflag = IPC_CREAT | IPC_EXCL | SVSEM_MODE;
        if ((semid = semget(ftok(LOCK_PATH, 0), 1, oflag)) >= 0) {
            /* sucess, we're first so initialize */
            arg.val = 1;
            if (semctl(semid, 0, SETVAL, arg) < 0)
                err_sys("semctl error");
        } else if (errno == EEXIST) {
            /* someone else has created; make sure it's initialized */
            if ((semid = semget(ftok(LOCK_PATH,0), 1, SVSEM_MODE)) < 0)
                err_sys("semget error");
            arg.buf = &seminfo;
            for (i = 0; i < MAX_TRIES; i++) {
                if (semctl(semid, 0, IPC_STAT, arg) < 0)
                    err_sys("semctl error");
                if (arg.buf->sem_otime != 0)
                    goto init;
                sleep(1);
            }
            err_quit("semget OK, but semaphore not initialized");
        } else {
            err_sys("semget error");
        }
init:
        initflag = 1;
        postop.sem_num = 0;
        postop.sem_op = 1;
        postop.sem_flg = SEM_UNDO;
        waitop.sem_num = 0;
        waitop.sem_op = -1;
        waitop.sem_flg = SEM_UNDO;
    }
    semop(semid, &waitop, 1); /* down by 1 */
}

void my_unlock(int fd)
{
    semop(semid, &postop, 1); /* up by 1 */
}

int main(void)
{
    int fd;
    pid_t pid;

    fd = open("test.file", O_RDWR|O_CREAT, FILE_MODE);
    if (fd < 0)
        err_sys("open error");
    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) { /* child */
        my_lock(fd);
        printf("child got the lock\n");
        my_unlock(fd);
    } else { /* parent */
        my_lock(fd);
        printf("parent got the lock\n");
        sleep(10);
        my_unlock(fd);
    }
    exit(0);
}
