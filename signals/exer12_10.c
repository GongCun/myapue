#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include "myapue.h"

#define BUFFSIZE 655320

static void sig_alrm(int);

int main(void)
{
    int in;
    char buf[BUFFSIZE];
    int i;

    if (signal(SIGALRM, sig_alrm) == SIG_ERR)
    {
        perror("signal(SIGALRM)");
        return -1;
    }

    in = creat("./exer12.data", FILE_MODE);
    if (!in) {
        perror("creat error");
        return -1;
    }
    if (unlink("./exer12.data") < 0) {
        perror("unlink error");
        exit(1);
    }

    /* printf("%ld\n", (long)strlen(buf)); */
    for (i = 0; i < BUFFSIZE; i++)
        buf[i] = '0';
    /* printf("%ld\n", (long)strlen(buf)); */
    alarm(1);
    /* 
     * wait more than one second 
     */
    for (i = 0; i < 50000; i++)  
        if (write(in, buf, strlen(buf)) < 0) {
            perror("write");
            return -1;
        }
    printf("finished\n");
    exit(0);
}

static void sig_alrm(int signo)
{
    printf("\ncaught signal %d\n", signo);
    return;
}
