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
#include <signal.h>
#include "myapue.h"

#define MILION 1000000L

static void sleep_us(long);
static void sig_int(int);

int main(int argc, char **argv)
{
	if (argc != 2) {
		fprintf(stderr, "usage: %s microseconds\n", *argv);
        exit(1);
    }
    if (signal(SIGINT, sig_int) == SIG_ERR)
        err_sys("SIGINT error");
    usleep(atol(argv[1]));
    printf("usleep finished\n");
    sleep_us(atol(argv[1]));
    printf("sleep_us finished\n");
    exit(0);
}

static void sleep_us(long usec)
{
	struct timeval timeout;
    int retval;

    timeout.tv_sec = usec/MILION; /* int part */
    timeout.tv_usec = usec%MILION; /* remainder */
    retval = select(0, NULL, NULL, NULL, &timeout);
    if (retval != 0)
        err_sys("select failed");
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
