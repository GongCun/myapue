#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "myapue.h"

static void f_vfork(int *);
int main(void)
{
    int x = 0;

    f_vfork(&x);
    printf("x = %d\n", x);
    return 0;
}

static void f_vfork(int *x)
{
    pid_t pid;
    int status;

    if ((pid = vfork()) < 0)
        err_sys("vfork error");
    else if (pid == 0) {
        *x += 1;
        _exit(0);
    }

    if (wait(&status) != pid)
        err_sys("wait error");

    printf("vfork\n");
}
