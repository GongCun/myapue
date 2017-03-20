#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include "myapue.h"

extern void pr_exit(int);

int globvar = 6; /* external variable in initialized data */

int main(void)
{
    int var; /* automatic variable on the stack */
    int i;
    int status;

    char buf[32];

    pid_t pid;

    var = 88;
    printf("before fork\n"); /* we don't flush stdout */


    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {
        globvar++;
        var++;
        fclose(stdout);
        exit(0);
    } 

    if (wait(&status) != pid)
        err_sys("wait error");
    pr_exit(status);

    /* parent continues here */
    i = printf("pid = %ld, glob = %d, var = %d\n", (long)getpid(), globvar, var);
    sprintf(buf, "i = %d\n", i);
    write(1, buf, strlen(buf));
    
    return 0;
}


