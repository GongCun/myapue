#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include "myapue.h"

static void f(void);

int main(void)
{
    pid_t pid;

    printf("program start\n"); /* parent process */

    if ((pid = fork()) < 0)
        err_sys("fork error");
    
    else if (pid == 0) {
        printf("child process %ld\n", (long)getpid()); 
        sleep(10);
        _exit(0);
    } 
    else {
        printf("parent process %ld\n", (long)getpid()); 
    }

    /* the code below is only for parent because child process _exit */
    if (wait(NULL) < 0)
        err_sys("wait error");

    printf("pid = %ld\n", (long)getpid()); /* share process */
    f();
    printf("program end\n");

    return 0;
}

static void f(void)
{
    printf("f()\n");
    return;
}
