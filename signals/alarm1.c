#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

static void sig_alarm(int);

int main(void)
{
    int i;

    signal(SIGALRM, sig_alarm);
    alarm(5);

    for (i = 1; i < 10; i++ )
    {
        printf("%d\n", i);
        sleep(1);
    }

    return 0;
}

static void sig_alarm(int signo)
{
    printf("alarm\n");
    return;
}
