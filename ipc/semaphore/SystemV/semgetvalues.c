#include "myapue.h"
#include <sys/sem.h>

int main(int argc, char **argv)
{
    int semid, nsems, i;
    struct semid_ds seminfo;
    unsigned short *ptr;
    union semun arg;

    if (argc != 2)
        err_quit("%s <pathname>", *argv);
    /* first get the number of semaphores in the set */
    if ((semid = semget(ftok(argv[1], 0), 0, 0)) < 0)
        err_sys("semget error");
    arg.buf = &seminfo;
    if (semctl(semid, 0, IPC_STAT, arg) < 0)
        err_sys("semctl error");
    nsems = arg.buf->sem_nsems;

    /* allocate memory to hold all the vaules in the set */
    if ((ptr = calloc(nsems, sizeof(unsigned short))) == NULL)
        err_sys("calloc error");
    arg.array = ptr;
    if (semctl(semid, 0, GETALL, arg) < 0)
        err_sys("semctl error");
    for (i = 0; i < nsems; i++)
        printf("semval[%d] = %d\n", i, ptr[i]);
    exit(0);
}
