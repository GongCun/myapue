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
#include <pthread.h>
#include "daemon.h"

#define BUFFSIZE 32 
#define RECORD "/Users/gongcunjust/c_program/sys_program/daemon_processes/daemon_date.txt"
#define CONFFILE "/Users/gongcunjust/c_program/sys_program/daemon_processes/daemon_conf.txt"

sigset_t mask;
void reread(void)
{
    char buf[16] = "reread test\n";
    int fd;
    fd = open(CONFFILE, O_APPEND|O_CREAT|O_RDWR, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
    if (fd < 0) {
        perror("open file failed");
        exit(1);
    }
    write(fd, buf, strlen(buf)+1);
    close(fd);
    return;
}

void *thr_fn(void *arg)
{
    int err, signo;
    for (;;) {
        err = sigwait(&mask, &signo);
        if (err != 0) {
            syslog(LOG_ERR, "sigwait failed");
            exit(1);
        }

        switch (signo) {
            case SIGHUP:
                syslog(LOG_INFO, "Re-reading configuration file");
                reread();
                break;
            case SIGTERM:
                syslog(LOG_INFO, "got SIGTERM; exiting");
                exit(0);
            default:
                syslog(LOG_INFO, "unexpected signal %d\n", signo);
        }
    }
    pthread_exit((void *)0);
}

int
main(int argc, char **argv)
{
	char           *cmd;
	int		fd;
    char buf[BUFFSIZE];
    time_t tp;
    struct tm *lt;
    struct sigaction sa;
    int err;
    pthread_t tid;

	if ((cmd = strrchr(argv[1], '/')) == NULL)
		cmd = argv[1];
	else
		cmd++;


	/* become a daemon */
	daemonize(cmd);
    syslog(LOG_NOTICE, "daemon (pid: %ld) started", (long)getpid());

	/* make sure only one copy of the daemon is running. */
	if (already_running()) {
		syslog(LOG_ERR, "daemon already running");
        return 1;
	}

    /*
     * restore SIGHUP default and block all signals
     */
    sa.sa_handler = SIG_DFL;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGHUP, &sa, NULL) < 0) {
        return 1;
    }
    sigfillset(&mask);
    if ((err = pthread_sigmask(SIG_BLOCK, &mask, NULL)) != 0) {
        syslog(LOG_ERR, strerror(err));
        return 1;
    }

    /*
     * create a thread to handle SIGHUP and SIGTERM
     */
    err = pthread_create(&tid, NULL, thr_fn, 0);
    if (err != 0) {
        syslog(LOG_ERR, strerror(err));
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
