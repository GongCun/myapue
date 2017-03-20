#include "myapue.h"
#include <sys/sem.h>
#include <stdarg.h>

#define LOCK_PATH "./svsemlock"
#define MAX_TRIES 10

int semcreate(int nsems)
{
    int oflags, semid;

    oflags = SVSEM_MODE | IPC_CREAT | IPC_EXCL;
    if ((semid = semget(IPC_PRIVATE, nsems, oflags)) < 0)
        err_sys("semget error");
    return(semid);
}

void semrmid(int semid)
{
    if (semctl(semid, 0, IPC_RMID) < 0)
        err_sys("semctl(IPC_RMID) error");
    return;
}

void semsetvalues(int semid, unsigned short *val, int vlen)
{
    int nsems;
    struct semid_ds seminfo;
    union semun arg;

    arg.buf = &seminfo;
    if (semctl(semid, 0, IPC_STAT, arg) < 0)
        err_sys("semctl(IPC_STAT), error");
    nsems = arg.buf -> sem_nsems;
    if (nsems != vlen)
        err_quit("nsems(%d) != array length(%d)", nsems, vlen);

    arg.array = val;
    if (semctl(semid, 0, SETALL, arg) < 0)
        err_sys("semctl(SETALL) error");
    return;
}

void semgetvalues(int semid)
{
    int i, nsems;
    struct semid_ds seminfo;
    union semun arg;
    unsigned short *ptr;

    arg.buf = &seminfo;
    if (semctl(semid, 0, IPC_STAT, arg) < 0)
        err_sys("semctl error");
    nsems = arg.buf -> sem_nsems;
    if ((ptr = calloc(nsems, sizeof(unsigned short))) == NULL)
        err_sys("calloc error");
    arg.array = ptr;

    if (semctl(semid, 0, GETALL, arg) < 0)
        err_sys("semctl error");
    for (i = 0; i < nsems; i++)
        printf("semval[%d] = %d\n", i, ptr[i]);
    printf("\n\n");
    return;
}
