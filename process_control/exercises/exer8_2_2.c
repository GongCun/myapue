#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "myapue.h"

static void f1(void), f2(void);

int main(void) /* incorrect use of vfork */
{

    f1();
    f2();
    _exit(0);
}

static void f1(void)
{
    pid_t pid;

    if ((pid = vfork()) < 0)
        err_sys("vfork error");
    /* child and parent both return */
}

static void f2(void)
{
    char buf[1000]; /* automatic variables */
    int i;

    for (i = 0; i < 1000; i++)
        buf[i] = 0;
}
