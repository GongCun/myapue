#include "myapue.h"
#include <semaphore.h>

int main(int argc, char **argv)
{
    sem_t *sem;
    if (argc != 2)
        err_quit("%s <name>", *argv);
    if ((sem = sem_open(argv[1], 0)) == SEM_FAILED)
        err_sys("sem_open error");
    if (sem_wait(sem) < 0)
        err_sys("sem_wait error");
    printf("pid %ld has semaphore\n", (long)getpid());
    pause();
    exit(0);
}

