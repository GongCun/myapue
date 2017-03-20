#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>

#define BUFFSIZE 1

static void sig_alrm(int);

int main(void)
{
    FILE *in;
    unsigned long a[1024][1024][4];
    char buf[BUFFSIZE];

    if (signal(SIGALRM, sig_alrm) == SIG_ERR)
    {
        perror("signal(SIGALRM");
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
