#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "myapue.h"

extern void pr_ids(char *);

int main(void)
{
    const char *str = "write test\n";
    char buf[128];
    char c;
    int len;
    int fd;
    pid_t pid;

    len = strlen(str);
    if (write(STDOUT_FILENO, str, len) != len)
        err_sys("write error");
    if (read(STDIN_FILENO, &c, 1) != 1)
        err_sys("read error");

    printf("c = %c\n", c);

    if ((pid = fork()) < 0)
        err_sys("fork error");
    else if (pid == 0) {
        pr_ids("before");
        if (setsid() == -1)
            err_sys("setsid error");
        fd = open("./exer9_2.out", O_RDWR);
        if (fd == -1)
            err_sys("open error");
        ftruncate(fd, 0);
        sprintf(buf, "after: pid = %ld, ppid = %ld, pgrp = %ld, tpgid = %ld",
                (long)getpid(), (long)getppid(), (long)getpgid(0), (long)tcgetpgrp(STDIN_FILENO));
        write(fd, buf, strlen(buf) + 1);
        close(fd);
        if (read(STDIN_FILENO, &c, 1) != 1)
            err_sys("read errno: %d", errno);

        exit(0);
    }

    if (wait(NULL) < 0)
        err_sys("wait error");
    system("cat ./exer9_2.out");
    fputc('\n', stdout);
    return 0;
}
