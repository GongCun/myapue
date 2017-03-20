#include <setjmp.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include "myapue.h"

extern unsigned int sleep3(unsigned int);
static void sig_int(int);

int main(void)
{
    unsigned int unslept;

    if (signal(SIGINT, sig_int) == SIG_ERR)
        err_sys("signal(SIGINT) error");
    unslept = sleep3(5); /* test real sleep */
    printf("sleep3 returned: %u\n", unslept);
    return 0;
}

static void sig_int(int signo)
{
    int i, j;
    volatile int k; /* avoid optimizing compiler */

    /* Tune these loops to run for more than 5 seconds */
    printf("\nsig_int starting\n");
    for (i = 0; i < 60000; i++)
        for (j = 0; j < 400000; j++)
            k += i * j;
    printf("sig_int finished\n");
}
