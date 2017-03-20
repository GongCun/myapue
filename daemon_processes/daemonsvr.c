#include <sys/types.h>
#include <pwd.h>
#include <uuid/uuid.h>
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
#include <pthread.h>
#include "daemon.h"

#define err_sys(msg) { syslog(LOG_ERR, msg); exit(1); }

pid_t pid;

void sig_term(int signo)
{
    syslog(LOG_NOTICE, "got SIGTERM; exiting");
    if (pid != 0) {
        if (kill(pid, SIGTERM) < 0) { /* the SIGTERM is reset to SIG_DFL */
            err_sys("kill server error");
        } else
            syslog(LOG_NOTICE, "server got SIGTERM; exiting");
    }
    exit(0);
}

int
main(int argc, char **argv)
{
	char           *cmd;

    if (argc != 3) {
        fprintf(stderr, "%s <command> <dirname>\n", *argv);
        exit(1);
    }
	if ((cmd = strrchr(argv[1], '/')) == NULL)
		cmd = argv[1];
	else
		cmd++;

	/* become a daemon */
	daemonize(cmd);
    syslog(LOG_NOTICE, "daemon (pid: %ld) started", (long)getpid());

	/* make sure only one copy of the daemon is running. */
	if (already_running()) 
        err_sys("daemon already running");

    if (signal(SIGTERM, sig_term) == SIG_ERR)
        err_sys("signal error");

	/* do its daemon thing */
    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {
        if (chdir(argv[2]) < 0)
            err_sys("chdir error");
        if (execl(argv[1], argv[1], (char *) 0) < 0)
            err_sys("execl error");
        _exit(127); /* shouldn't get here */
    }
    if (waitpid(pid, NULL, 0) < 0)
        err_sys("waitpid error");

	return 0;
}
