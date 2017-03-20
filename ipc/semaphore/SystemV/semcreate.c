#include "myapue.h"
#include <sys/sem.h>

int main(int argc, char **argv)
{
    int c, oflag, semid, nsems;

    oflag = SVSEM_MODE | IPC_CREAT;
    while ((c = getopt(argc, argv, "e")) != -1) {
        switch (c) {
            case 'e':
                oflag |= IPC_EXCL;
                break;
        }
    }
    if (optind != argc - 2)
        err_quit("%s [-e] <pathname> <nsems>", *argv);
    nsems = atoi(argv[optind+1]);
    if ((semid = semget(ftok(argv[optind], 0), nsems, oflag)) < 0)
        err_sys("semget error");
    exit(0);
}
