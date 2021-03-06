#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "myapue.h"

char *env_init[] = { "USER=unknown", "PATH=/tmp", NULL };

int main(void)
{
    pid_t pid;

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {
        if (execle("/Users/gongcunjust/c_program/sys_program/process_control/echoall",
                    "echoall", "myarg", "MY ARG2", (char *)0, env_init) < 0)
            err_sys("execle error");
    }

    if (waitpid(pid, NULL, 0) < 0)
        err_sys("wait error");

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {
        if (execlp("echoall", "echoall", "only 1 arg", (char *)0) < 0) /* export PATH=$PATH:. */
            err_sys("execlp error");
    }

    return 0;
}


