#include "myapue.h"
#include <sys/sem.h>

int main(int argc, char **argv)
{
    int semid;

    if (argc != 2)
        err_quit("%s <pathname>", *argv);
    if ((semid = semget(ftok(argv[1], 0), 0, 0)) < 0)
        err_sys("semget error");
    if (semctl(semid, 0, IPC_RMID) < 0)
        err_sys("semctl error");
    exit(0);
}
            
