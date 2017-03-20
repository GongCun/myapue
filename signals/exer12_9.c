#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>

#define BUFFSIZE 65532

static void sig_alrm(int);

int main(void)
{
    FILE *in;
    long long a;
    char buf[BUFFSIZE];
    int i;

    if (signal(SIGALRM, sig_alrm) == SIG_ERR)
    {
        perror("signal(SIGALRM)");
        return -1;
    }

    in = fopen("./exer12.data", "w");
    if (!in) {
        perror("fopen");
        return -1;
    }
    if (unlink("./exer12.data") < 0) {
        perror("unlink error");
        exit(1);
    }

    if (setvbuf(in, buf, _IOFBF, BUFFSIZE) != 0)
    {
        perror("setvbuf");
        return -1;
    }

    alarm(1);
    /* 
     * wait more than one second 
     */
    a = 1234567890;
    for (i = 0; i < 100000000; i++)  
        if (!fwrite(&a, sizeof(a), 1, in)) {
            perror("fwrite");
            return -1;
        }
    exit(0);
}

static void sig_alrm(int signo)
{
    printf("\ncaught signal %d\n", signo);
    return;
}
