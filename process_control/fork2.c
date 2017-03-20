#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int globvar = 6; /* external variable in initialized data */

int main(void)
{
    int var; /* automatic variable on the stack */
    pid_t pid;

    var = 88;
    printf("before fork\n"); /* we don't flush stdout */


    if ((pid = vfork()) < 0) {
        perror("vfork error");
        exit(1);
    } else if (pid == 0) {
        printf("child\n");
        globvar++;
        var++;
#ifdef TEST
        exit(0);
#else
        _exit(0);
#endif
    } 

    /* parent continues here */
    printf("pid = %ld, glob = %d, var = %d\n", (long)getpid(), globvar, var);

    return 0;
}


