#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>

/* can't more than the system's stack size */
#define BUFFSIZE 60000000

static void sig_alrm(int);

int main(void)
{
    FILE *in;
    unsigned long a[1024][512];
    char buf[BUFFSIZE];
    int i;

    if (signal(SIGALRM, sig_alrm) == SIG_ERR)
    {
        perror("signal(SIGALRM)");
        return -1;
    }

    alarm(1);
    in = fopen("./exer12.data", "w");
    if (!in) {
        perror("fopen");
        return -1;
    }

    if (setvbuf(in, buf, _IOFBF, BUFFSIZE) != 0)
    {
        perror("setvbuf");
        return -1;
    }

    /* 
     * wait more than one second 
     * total bytes: 125 * 1024 * 512 * 8
     */
    for (i = 0; i < 125; i++)  
        if (!fwrite(&a, sizeof(a), 1, in)) {
            perror("fwrite");
            return -1;
        }

    if (fclose(in)) {
        perror("fclose");
        return -1;
    }
    alarm(0);

    return 0;
}

static void sig_alrm(int signo)
{
    printf("\ncaught SIGALRM\n");
    return;
}
