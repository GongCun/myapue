#include "myapue.h"
#include <fcntl.h>
#include <errno.h>
#include <signal.h>

#define LOCKFILE "temp.lock"
#define TEMPFILE "temp.file"
#define OPEN_MODE (O_RDWR|O_TRUNC|O_CREAT)

static void sig_alrm(int signo);
int fd, rdwrfd;

int main(int argc, char **argv)
{
    pid_t pid;

    if (argc != 2) {
        fprintf(stderr, "%s <raise-interval>\n", argv[0]);
        exit(1);
    }
    if ((fd = open(TEMPFILE, OPEN_MODE, FILE_MODE)) < 0)
        err_sys("open TEMPFILE error");
    if ((rdwrfd = open(LOCKFILE, OPEN_MODE, FILE_MODE)) < 0)
        err_sys("open LOCKFILE error");
    if (signal(SIGALRM, sig_alrm) == SIG_ERR)
        err_sys("signal(SIGALRM) error");
    set_ticker(atoi(argv[1]));

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {
        if (write_lock(rdwrfd, 0, SEEK_SET, 0) < 0)
            err_sys("write_lock LOCKFILE error");
        if (writew_lock(fd, 0, SEEK_SET, 0) < 0)
            err_sys("writew_lock TEMPFILE error");
        printf("got the write lock of TEMPFILE\n");
        if (un_lock(fd, 0, SEEK_SET, 0) < 0)
            err_sys("un_lock TEMPFILE error");
        if (un_lock(rdwrfd, 0, SEEK_SET, 0) < 0)
            err_sys("un_lock LOCKFILE error");
    } else {
        if (readw_lock(fd, 0, SEEK_SET, 0) < 0)
            err_sys("readw error");
        sleep(2);
        if (un_lock(fd, 0, SEEK_SET, 0) < 0)
            err_sys("un_lock error");
        printf("released the read lock\n");
        if (waitpid(pid, NULL, 0) < 0)
            err_sys("wait write process error");
        for(;;)
            pause();
    }
    exit(0);
}

static void sig_alrm(int signo)
{
    pid_t pid;
    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {
        while(readw_lock(rdwrfd, 0, SEEK_SET, 0) < 0) {
            if (errno == EACCES || errno == EAGAIN)
                continue;
            else
                err_sys("readw_lock LOCKFILE error");
        }
        if (readw_lock(fd, 0, SEEK_SET, 0) < 0)
            err_sys("queue readw error");
        sleep(2);
        if (un_lock(fd, 0, SEEK_SET, 0) < 0)
            err_sys("un_lock TEMPFILE error");
        printf("released the read lock\n");
        if (un_lock(rdwrfd, 0, SEEK_SET, 0) < 0)
            err_sys("un_lock LOCKFILE error");
        exit(0);
    }
    if (waitpid(pid, NULL, 0) < 0)
        err_sys("waitpid error");
    return;
}
