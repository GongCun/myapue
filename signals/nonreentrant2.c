#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>
#include <signal.h>
#include "myapue.h"

const static char *buf = "signal handler\n";
static void my_alarm(int signo)
{
    write(STDOUT_FILENO, buf, 16); /* reentrant function */
    alarm(1);
    return;
}

int main(void)
{
    struct passwd *ptr;

    signal(SIGALRM, my_alarm);
    alarm(1);
    for ( ; ; ) {
        if ((ptr = getpwnam("gongcunjust")) == NULL)
            err_sys("getpwnam error");
        if (strcmp(ptr->pw_name, "gongcunjust") != 0)
            printf("return value corrupted!, pw_name = %s\n",
                    ptr->pw_name);
    }
    return 0;
}
