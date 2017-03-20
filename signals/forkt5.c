#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include "myapue.h"

static void f(void);
static void sig_int(int);

int main(void)
{
    pid_t pid;

    if (signal(SIGINT, sig_int) == SIG_ERR)
        err_sys("signal(SIGINT) error");

    printf("program start\n"); /* parent process */

    if ((pid = fork()) < 0)
        err_sys("fork error");
    
    else if (pid == 0) {
        printf("child process %ld\n", (long)getpid()); 
        f(); /* if here ctrl-c will be caught twice: parent and child */
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
    printf("f() sleep 20s\n");
    sleep(20);
    return;
}

static void sig_int(int signo)
{
    printf("\ncaught SIGINT\n");
    return;
}
