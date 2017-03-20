#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <poll.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include "myapue.h"

static void sleep_us(int);
static void sig_int(int);

int main(int argc, char **argv)
{
	if (argc != 2) {
		fprintf(stderr, "usage: %s microseconds\n", *argv);
        exit(1);
    }
    if (signal(SIGINT, sig_int) == SIG_ERR)
        err_sys("SIGINT error");
    sleep_us(atoi(argv[1]));
    exit(0);
}

static void sleep_us(int usec)
{
    int timeout, retval;

    if ((timeout = usec/1000) <= 0)
        timeout = 1;
    retval = poll(NULL, 0, timeout); /* millisecond */
    if (retval != 0)
        err_sys("poll failed");
    return;
}

static void sig_int(int signo)
{
    int i,j;
    volatile int k;
    /*
     * tune these loops to run for more than 5 seconds
     */
    printf("\nsig_int starting\n");
    for (i = 0; i < 300000; i++)
        for (j = 0; j < 6000; j++)
            k += i*j;
    printf("\nsig_int finished\n");
    return;
}
