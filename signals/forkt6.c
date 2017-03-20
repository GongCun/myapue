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
    sigset_t newmask, oldmask;

    printf("program start\n"); /* parent process */

    if (signal(SIGINT, sig_int) == SIG_ERR)
        err_sys("signal(SIGINT) error");

    /*
     * block SIGINT and save current signal mask
     */
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGINT);
    if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)
        err_sys("SIG_BLOCK error");


    if ((pid = fork()) < 0)
        err_sys("fork error");
    
    else if (pid == 0) {
        printf("child process %ld\n", (long)getpid()); 

        /*
         * reset signal mask which unblocks SIGINT
         */
        if (sigprocmask(SIG_SETMASK, &oldmask, NULL))
            err_sys("SIG_SETMASK error");

        f(); /* if here ctrl-c will be caught once: child */
        _exit(0);
    } 
    else {
        printf("parent process %ld\n", (long)getpid()); 
    }

    /* the code below is only for parent because child process _exit */
    if (wait(NULL) < 0)
        err_sys("wait error");

    printf("pid = %ld\n", (long)getpid()); /* share process */
    f(); /* here ctrl-c will be blocked */
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
