#include <sys/poll.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

#define err_exit(msg, rc) { perror(msg); exit(rc); }
static int openfile(char *);
static void showdata(char *, int);

int main(int argc, char **argv)
{
    struct pollfd fds[2];
    int fd1, fd2, ret;

    if (argc != 4) {
    	fprintf(stderr, "usage: %s file file timeout\n", argv[0]);
    	exit(1);
	}

    if ( (fd1 =  openfile(argv[1])) < 0 )
    	err_exit("open error", 1);
    if ( (fd2 = openfile(argv[2])) < 0 )
    	err_exit("open error", 2);

    while (1) {
		fds[0].fd = fd1;
    	fds[0].events = POLLIN;
    	fds[1].fd = fd2;
    	fds[1].events = POLLIN;

    	ret = poll(fds, 2, atoi(argv[3]) * 1000);
    	if (ret == -1) {
    		err_exit("poll error", 1);
    	}

        if (ret > 0) {
        	if (fds[0].revents & POLLIN)
        		showdata(argv[1], fd1);
        	if (fds[1].revents & POLLIN)
        		showdata(argv[2], fd2);
        } else {
            printf("%d seconds elapsed.\n", atoi(argv[3]));
    	}
	}
    exit(0);
}

static void showdata(char *fname, int fd)
{
	char buf[BUFSIZ];
	int n;

	printf("%s:\n", fname); 
	fflush(stdout);
	if ( (n = read(fd, buf, BUFSIZ)) < 0 ) {
		err_exit("read error", 1);
	}
	buf[strlen(buf)-1] = 0;
	if (write(STDOUT_FILENO, buf, n) != n)
		err_exit("write error", 1);

	putchar('\n');
}

static int openfile(char *fname)
{
	int fd;
	struct stat statbuf;

	if (stat(fname, &statbuf) < 0)
		err_exit("stat error", 1);
	if (S_ISFIFO(statbuf.st_mode)) {
		fd = open(fname, O_RDWR|O_NONBLOCK);
	} else {
		fd = open(fname, O_RDWR);
	}
	return(fd);
}
