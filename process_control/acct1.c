#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include "myapue.h"

int main(void)
{
    pid_t pid;

    if ((pid = fork()) < 0)
        err_sys("fork error");
    else if (pid != 0) {
        sleep(2);
        exit(2); /* parent terminate with exit status 2 */
    }

    if ((pid = fork()) < 0)
        err_sys("fork error");
    else if (pid != 0) { /* first child */
        sleep(4);
        abort();
    }

    if ((pid = fork()) < 0)
        err_sys("fork error");
    else if (pid != 0) { /* second child */
        execl("/bin/dd", "dd", "if=/tmp/x", "of=/dev/null", NULL);
        exit(7); /* shouldn't get here */
    }

    if ((pid = fork()) < 0)
        err_sys("fork error");
    else if (pid != 0) { /* third child */
        sleep(8);
        exit(0);
    }

    sleep(6); /* fourth child */
    kill(getpid(), SIGKILL);

    return 6;
}
    
