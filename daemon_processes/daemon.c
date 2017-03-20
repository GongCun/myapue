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
#include "daemon.h"

#define BUFFSIZE 16
extern int
		daemonize     (const char *);
extern int
		already_running(const char *);

static void 
sigterm(int signo)
{
	syslog(LOG_INFO, "got SIGTERM; exiting");
	exit(0);
}

int
main(int argc, char **argv)
{
	char           *cmd;
	int		interval = 1;
	int		fd;
	char		pid       [16];
	struct sigaction sa;

	if (argc < 2) {
		printf("missing arguments : use <-h> to help\n");
		return 1;
	}
	if (strcmp(argv[1], "-h") == 0) {
		printf("usage -- start daemon: daemon <command's full path> <interval seconds>\n");
		printf("usage -- stop daemon: daemon -k\n");
		return 0;
	}
	if (strcmp(argv[1], "-k") == 0) {
		fd = open(LOCKFILE, O_RDONLY);
		if (fd < 0) {
			printf("Can't open %s: %s\n", LOCKFILE, strerror(errno));
			exit(1);
		}
		if (read(fd, pid, BUFFSIZE) <= 0) {
			printf("Can't read %s: %s\n", LOCKFILE, strerror(errno));
			exit(1);
		}
		if (kill(atol(pid), 15) < 0) {
			printf("Can't kill %s: %s\n", pid, strerror(errno));
			exit(1);
		}
		printf("daemon process %s terminated\n", pid);
		return 0;
	}
	if ((cmd = strrchr(argv[1], '/')) == NULL)
		cmd = argv[1];
	else
		cmd++;

	if (argc == 3)
		interval = atoi(argv[2]);

	printf("start daemon %s, detail on the /var/adm/log/syslog/syslog.out\n", cmd);

	/* become a daemon */
	daemonize(cmd);

	/* make sure only one copy of the daemon is running. */
	if (already_running(cmd)) {
		syslog(LOG_ERR, "daemon already running");
		exit(1);
	}
	/* handle signal of termination */
	sa.sa_handler = sigterm;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGTERM, &sa, NULL) < 0) {
		syslog(LOG_ERR, "Can;t catch SIGTERM: %s", strerror(errno));
		exit(1);
	}
	/* do its daemon thing */
	cmd = argv[1];
	while (1) {
		if (system(cmd) != 0) {
			syslog(LOG_EMERG, "cmd %s error, daemon abort", cmd);
			exit(1);
		}
		sleep(interval);
	}

	return 0;
}
