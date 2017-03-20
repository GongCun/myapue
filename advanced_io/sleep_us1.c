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
#include "myapue.h"

#define MILION 1000000L

static void sleep_us(long);

int main(int argc, char **argv)
{
	if (argc != 2) {
		fprintf(stderr, "usage: %s microseconds\n", *argv);
        exit(1);
    }
    sleep_us(atol(argv[1]));
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
