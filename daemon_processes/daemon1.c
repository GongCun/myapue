#include <sys/types.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <syslog.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include "daemon.h"

#define BUFFSIZE 32 
#define RECORD "/Users/gongcunjust/c_program/sys_program/daemon_processes/daemon_date.txt"

int
main(int argc, char **argv)
{
	char           *cmd;
	int		fd;
    char buf[BUFFSIZE];
    time_t tp;
    struct tm *lt;

	if ((cmd = strrchr(argv[1], '/')) == NULL)
		cmd = argv[1];
	else
		cmd++;

    syslog(LOG_NOTICE, "daemon (pid: %ld) started", (long)getpid());

	/* become a daemon */
	daemonize(cmd);

	/* make sure only one copy of the daemon is running. */
	if (already_running()) {
		syslog(LOG_ERR, "daemon already running");
        return 1;
	}

	/* do its daemon thing */
	while (1) {
        time(&tp);
        lt = localtime(&tp);
        fd = open(RECORD, O_APPEND|O_CREAT|O_RDWR, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
        if (fd < 0) {
            syslog(LOG_ERR, "open record file error");
            return 1;
        }
        sprintf(buf, "%4d/%2d/%2d %2d:%2d:%2d\n",
                lt->tm_year + 1900,
                lt->tm_mon + 1,
                lt->tm_mday,
                lt->tm_hour,
                lt->tm_min,
                lt->tm_sec);
        write(fd, buf, strlen(buf)+1);
        close(fd);
		sleep(1);
	}

	return 0;
}
