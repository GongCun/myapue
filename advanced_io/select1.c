#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#define perr_exit(m, x) { perror(m); exit(x); }

static void showdata(char *, int);
static int openfile(char *);
static void sig_hup(int signo);

int main(int argc, char **argv)
{
    int fd1, fd2;
	struct timeval timeout;
	fd_set readfds;
	int maxfd;
	int retval;

    if (signal(SIGHUP, sig_hup) == SIG_ERR)
        perr_exit("signal(SIGHUP) error", 1);

	if (argc != 4) {
		fprintf(stderr, "usage: %s file file timeout\n", *argv);
        exit(1);
    }

    /* open file */
    if ( (fd1 = openfile(argv[1])) < 0 )
        perr_exit("open error", 2);
    if ( (fd2 = openfile(argv[2])) < 0 )
        perr_exit("open error", 3);
    maxfd = 1 + (fd1 > fd2 ? fd1 : fd2);

    while (1) {
        /* 
         * make a list of file descriptors
         */
        FD_ZERO(&readfds);
        FD_SET(fd1, &readfds);
        FD_SET(fd2, &readfds);

        /*
         * set timeout value
         */
        timeout.tv_sec = atoi(argv[3]);
        timeout.tv_usec = 0;

        /* wait for input */
        retval = select(maxfd, &readfds, NULL, NULL, &timeout);
        if (retval == -1)
            perr_exit("select", 4);
        if (retval > 0) {
            if (FD_ISSET(fd1, &readfds))
                showdata(argv[1], fd1);
            if (FD_ISSET(fd2, &readfds))
                showdata(argv[2], fd2);
        } else {
            printf("no input after %d seconds\n", atoi(argv[3]));
        }
    }
}

static void showdata(char *fname, int fd)
{
    char buf[BUFSIZ];
    int n;
    printf("%s:\n", fname);
    if ( (n = read(fd, buf, BUFSIZ)) < 0 )
        perr_exit("read", 5);
    if ( write(STDOUT_FILENO, buf, n) != n )
        perr_exit("write", 6);
    putchar('\n');
}

static int openfile(char *fname)
{
    int fd;
    struct stat statbuf;

    if (stat(fname, &statbuf) < 0)
        perr_exit("stat error", 1);
    if ( S_ISFIFO(statbuf.st_mode) || S_ISCHR(statbuf.st_mode) ) {
        fd = open(fname, O_RDWR|O_NONBLOCK);
    } else {
        fd = open(fname, O_RDWR);
    }
    return(fd);
}

static void sig_hup(int signo)
{
    printf("caught SIGHUP\n");
    return;
}
