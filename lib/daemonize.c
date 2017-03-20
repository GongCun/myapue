#include "myapue.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <syslog.h>

int
daemonize(const char *cmd)
{
	int		i         , fd0, fd1, fd2;
	pid_t		pid;
	struct rlimit	rl;
    struct sigaction sa;

	umask(0);		/* clear file creation mask */

	if (getrlimit(RLIMIT_NOFILE, &rl) < 0)
		return 1;

	pid = fork();
	if (pid < 0)
		return 1;
	else if (pid != 0)
		exit(EXIT_SUCCESS);

	if (setsid() < 0)
		return 1;

    /*
     * ensure future open won't allocate controlling TTYs
     */
    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGHUP, &sa, NULL) < 0) {
        perror("sigaction");
        return 1;
    }
    if ((pid = fork()) < 0) {
        perror("fork");
        return 1;
    }
    else if (pid != 0) /* parent */
        exit(EXIT_SUCCESS);


	/*
	 * change the current working directory to the root so we won't
	 * prevent file system from being unmounted.
	 */

    if (chdir("/") < 0)
        return 1;

	/* close all open file descriptions */
	if (rl.rlim_max == RLIM_INFINITY)
		rl.rlim_max = 1024;
	for (i = 0; i < (int)rl.rlim_max; i++)
		close(i);

	/* redirect fd's 0, 1, 2 to /dev/null */
	fd0 = open("/dev/null", O_RDWR);	/* 0 stdin */
	fd1 = dup(0);		/* 1 stdout */
	fd2 = dup(0);		/* 2 stderr */

	/* initialize the log file. */
	openlog(cmd, LOG_CONS, LOG_DAEMON);
	if (fd0 != 0 || fd1 != 1 || fd2 != 2) {
		syslog(LOG_ERR, "unexpected file description %d %d %d", fd0, fd1, fd2);
		return 1;
	}
	return 0;
}
