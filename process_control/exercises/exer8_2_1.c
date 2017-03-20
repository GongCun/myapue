#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "myapue.h"

static void f_fork(int *);
int main(void)
{
    int x = 0;

    f_fork(&x);
    printf("x = %d\n", x);
    return 0;
}

static void f_fork(int *x)
{
    pid_t pid;
    int status;

    if ((pid = fork()) < 0)
        err_sys("fork error");
    else if (pid == 0) {
        *x += 1;
        _exit(0);
    }

    if (wait(&status) != pid)
        err_sys("wait error");

    printf("fork\n");
}
