#include "myapue.h"
#include <fcntl.h>
#include <errno.h>
#include <signal.h>

static void sig_alrm(int signo);
int fd;

int main(int argc, char **argv)
{
    pid_t pid;

    if (argc != 2) {
        fprintf(stderr, "%s <raise-interval>\n", argv[0]);
        exit(1);
    }
    if ((fd = open("rdwrlock.file", O_RDWR|O_TRUNC|O_CREAT, FILE_MODE)) < 0)
        err_sys("creat error");
    if (write(fd, "a", 1) != 1)
        err_sys("write error");
    if (signal(SIGALRM, sig_alrm) == SIG_ERR)
        err_sys("signal(SIGALRM) error");
    set_ticker(atoi(argv[1]));

    TELL_WAIT();
    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {
        WAIT_PARENT();
        if (writew_lock(fd, 0, SEEK_SET, 0) < 0)
            err_sys("writew error");
        printf("got the write lock\n");
        TELL_PARENT(getppid());
    } else {
        if (readw_lock(fd, 0, SEEK_SET, 0) < 0)
            err_sys("readw error");
        sleep(2);
        if (un_lock(fd, 0, SEEK_SET, 0) < 0)
            err_sys("un_lock error");
        printf("released the read lock\n");
        TELL_CHILD(pid);
        WAIT_CHILD();
    }
    exit(alarm(0));
}

static void sig_alrm(int signo)
{
    pid_t pid;
    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {
        if (readw_lock(fd, 0, SEEK_SET, 0) < 0)
            err_sys("queue readw error");
        sleep(2);
        if (un_lock(fd, 0, SEEK_SET, 0) < 0)
            err_sys("un_lock, error");
        printf("released the read lock\n");
        exit(0);
    }
    if (waitpid(pid, NULL, 0) < 0)
        err_sys("waitpid error");
    return;
}
