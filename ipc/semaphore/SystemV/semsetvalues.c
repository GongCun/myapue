#include "myapue.h"
#include <sys/sem.h>

int main(int argc, char **argv)
{
    int semid, nsems, i;
    struct semid_ds seminfo;
    unsigned short *ptr;
    union semun arg;

    if (argc < 2)
        err_quit("%s <pathname> [values ...]", *argv);
    /* first get the number of semaphores in the set */
    if ((semid = semget(ftok(argv[1], 0), 0, 0)) < 0)
        err_sys("semget error");
    arg.buf = &seminfo;
    if (semctl(semid, 0, IPC_STAT, arg) < 0)
        err_sys("semctl error");
    nsems = arg.buf->sem_nsems;

    /* now get the values from the command line */
    if (argc != nsems + 2)
        err_quit("%d semaphores in set, %d values specified", nsems, argc-2);

    /* allocate memory to hold all the vaules in the set, and store */
    if ((ptr = calloc(nsems, sizeof(unsigned short))) == NULL)
        err_sys("calloc error");
    arg.array = ptr;
    for (i = 0; i < nsems; i++)
        ptr[i] = atoi(argv[i+2]);
    if (semctl(semid, 0, SETALL, arg) < 0)
        err_sys("semctl error");
    exit(0);
}
