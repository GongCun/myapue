#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "myapue.h"

int main(void)
{
    pid_t pid;

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid ==0) { /* first child */
        if ((pid = fork()) < 0)
            err_sys("fork error");
        else if (pid > 0) {
            sleep(5);
            exit(0); /* parent from second fork == first child */
        }

        /*
         * we are the second child; our parent become init as soon
         * as our real parent calls exit() in the statement above.
         * here's where we'd continue executing, knowing that when
         * we're done, init will reap our status.
         */
        sleep(2);
        printf("second child, parent pid = %ld\n", (long)getppid());
        exit(0);
    }
    else
    {
        printf("original process = %ld\n", (long)getpid());
        printf("the first child process = %ld\n", (long)pid);
    }

    if (waitpid(pid, NULL, 0) != pid) /* wait for the first child */
        err_sys("waitpid error");

    /*
     * we're the parent (the original process); we continue executing,
     * knowing that we're not the parent of the second child.
     */
    return 0;
}

