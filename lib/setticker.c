#include "myapue.h"
#include <sys/time.h>

void set_ticker(int secs)
{
    struct itimerval delay;

    delay.it_value.tv_sec = secs;
    delay.it_value.tv_usec = 0;
    delay.it_interval.tv_sec = secs;
    delay.it_interval.tv_usec = 0;
    if (setitimer(ITIMER_REAL, &delay, NULL) < 0)
        err_sys("setitimer");
    pause();
    return;
}
