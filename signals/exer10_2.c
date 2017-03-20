#include <sys/resource.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "myapue.h"

#define RECORD "/Users/gongcunjust/c_program/sys_program/signals/tm_sec.txt"

int main(void)
{
    time_t tp;
    struct tm *lt;
    pid_t pid;
    struct rlimit rl;
    int i;
    int fd;
    char buf[32];

    /* ignore SIGHUP so disconnection will no interrupt the process */
    if (signal(SIGHUP, SIG_IGN) == SIG_ERR)
        return -1;

    /* get maximum number of file description */
    if (getrlimit(RLIMIT_NOFILE, &rl) < 0)
        err_quit("cant get file limit");
    
    if ((pid = fork()) < 0)
        err_sys("fork error");
    else if (pid != 0)
        exit(EXIT_SUCCESS);

    /* create new session group to lose controlling TTY */
    if (setsid() < 0)
        return -1;

    /* set the working directory to the root directory */
    if (chdir("/") < 0)
        return -1;

    /* close all open file */
    if (rl.rlim_max == RLIM_INFINITY)
        rl.rlim_max = 1024;
    for (i = 0; i < rl.rlim_max; i++)
        close(i);

    /* redirect fd's 0, 1, 2 to /dev/null */
    open("/dev/null", O_RDWR); /* stdin */
    dup(0); /* stdout */
    dup(0); /* stderror */


    /* daemon action */
    for (i = 0 ; ; i++ )
    {
        if (i%5 == 0) /* every 5 times fetch the current time */
        { 
            if (time(&tp) < 0)
                return -1;
            lt = localtime(&tp);

            fd = open(RECORD, O_RDWR|O_APPEND, 0);
            if (fd < 0)
                return -1;
            sprintf(buf, "%4d/%2d/%2d %2d:%2d:%2d\n", 
                    lt->tm_year + 1900, 
                    lt->tm_mon + 1,
                    lt->tm_mday,
                    lt->tm_hour,
                    lt->tm_min,
                    lt->tm_sec);

            write(fd, buf, strlen(buf)+1);
            close(fd);
        }
        sleep(60);
    }

    return 0;
}

