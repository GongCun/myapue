#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "myapue.h"

static void f(void);

int main(void)
{
    pid_t pid;

    printf("program\n"); /* parent process */

    if ((pid = fork()) < 0)
        err_sys("fork error");
    else if (pid == 0) {
        printf("child process %ld\n", (long)getpid()); /* only child process */
    } else {
        printf("parent process %ld\n", (long)getpid()); /* only parent process */
    }

    printf("pid = %ld\n", (long)getpid()); /* share process */
    f();
    printf("another process\n");

    return 0;
}

static void f(void)
{
    printf("f()\n");
    return;
}
