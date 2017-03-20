#include "myapue.h"
#include <sys/wait.h>

int main(void)
{
    char *cmd;
    pid_t pid;
    
    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) { /* child */
        cmd = "shutdown -s now";
        execl("/bin/sh", "sh", "-c", cmd, (char *)0);
        _exit(127);
    }
    if (waitpid(pid, NULL, 0) < 0)
        err_sys("waitpid error");
    printf("finished\n");
    exit(0);
}
