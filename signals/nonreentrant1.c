#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>
#include <signal.h>
#include "myapue.h"

static void my_alarm(int signo)
{
    struct passwd *rootptr;

    printf("in signal handler\n");
    if ((rootptr = getpwnam("root")) == NULL)
        err_sys("getpwnam(root) error");
    alarm(1); /* keep alarm */
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
