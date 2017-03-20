#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include "myapue.h"

extern void pr_exit(int);


int main(void)
{
    pid_t pid;
    int status;

    if ((pid = fork()) < 0)
        err_sys("fork error");
    else if (pid == 0) /* child */
        exit(7);

    if (wait(&status) != pid) /* wait for child */
        err_sys("wait error");
    pr_exit(status); /* print its status */

    if ((pid = fork()) < 0)
        err_sys("fork_error");
    else if (pid == 0)
        abort(); /* generates SIGABRT */
    
    if ((wait(&status) != pid))
        err_sys("wait error");
    pr_exit(status);

    if ((pid = fork()) < 0)
        err_sys("fork error");
    else if (pid == 0)
        status /= 0; /* divide by 0 generates SIGFPE */

    if ((wait(&status) != pid))
        err_sys("wait error");
    pr_exit(status);

    return 0;
}
            
            

