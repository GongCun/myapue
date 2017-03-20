#include "myapue.h"
#include <sys/sem.h>

int main(int argc, char **argv)
{
    int c, i, flag, semid, nops;
    struct sembuf *ptr;

    flag = 0;
    while ((c = getopt(argc, argv, "nu")) != -1) {
        switch (c) {
            case 'n':
                flag |= IPC_NOWAIT;
                break;
            case 'u':
                flag |= SEM_UNDO;
                break;
        }
    }
    if (argc - optind < 2) /* argc - optind = #args remaining */
        err_quit("%s [-n] [-u] <pathname> operation ...", *argv);

    if ((semid = semget(ftok(argv[optind], 0), 0, 0)) < 0)
        err_sys("semget error");
    optind++;
    nops = argc - optind;
    /* allocate memory to hold opeations, store, and perform */
    if ((ptr = calloc(nops, sizeof(struct sembuf))) == NULL)
        err_sys("calloc error");
    for (i = 0; i < nops; i++) {
        ptr[i].sem_num = i;
        ptr[i].sem_op = atoi(argv[nops+i]); /* <0, 0, >0 */
        ptr[i].sem_flg = flag;
    }
    if (semop(semid, ptr, nops) < 0)
        err_sys("semop error");
    exit(0);
}
