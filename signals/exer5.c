#include <sys/time.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void alarm_handler(int signo)
{
    printf("Timer hit!\n");
}

int main(void)
{
    struct itimerval delay;

    signal(SIGALRM, alarm_handler);

    delay.it_value.tv_sec = 5;
    delay.it_value.tv_usec = 0;
    delay.it_interval.tv_sec = 1;
    delay.it_interval.tv_usec = 0;
    if (setitimer(ITIMER_REAL, &delay, NULL) < 0) {
        perror("setitimer");
        return -1;
    }

    for ( ; ; ) 
        pause();
    return 0;
}
