#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include "myapue.h"

int globvar = 6; /* external variable in initialized data */

int main(void)
{
    int var; /* automatic variable on the stack */
    int i;

    char buf[32];

    pid_t pid;

    var = 88;
    printf("before fork\n"); /* we don't flush stdout */


    if ((pid = vfork()) < 0) {
        err_sys("vfork error");
    } else if (pid == 0) {
        globvar++;
        var++;
        fclose(stdout);
        exit(0);
    } 

    /* parent continues here */
    i = printf("pid = %ld, glob = %d, var = %d\n", (long)getpid(), globvar, var);
    sprintf(buf, "i = %d\n", i);
    write(1, buf, strlen(buf));
    return 0;
}


