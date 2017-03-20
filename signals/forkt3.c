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

    printf("program\n"); /* parent process */

    if ((pid = fork()) < 0)
        err_sys("fork error");
    else if (pid == 0) {
        printf("child process %ld\n", (long)getpid()); /* only child process */
        sleep(10);
    } 
    else {
        /*
         *
        if (waitpid(pid, NULL, 0) != pid)
            err_sys("waitpid error");

        while (waitpid(pid, NULL, 0) < 0)
            printf("waiting\n");

        */
        printf("parent process %ld\n", (long)getpid()); 
    }


    /* 
     * child process will wait pid==0 cause error
     *
    if (waitpid(pid, NULL, 0) != pid)
        err_sys("waitpid error"); 
    */

    if (wait(NULL) < 0)
        err_sys("wait error");

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
