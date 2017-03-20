#include "myapue.h"
#include <netdb.h>
#include <errno.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <limits.h>

#define BUFLEN 128
#define QLEN 10

#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX 256
#endif

static void sig_chld(int signo)
{
    while (waitpid((pid_t)-1, NULL, WNOHANG) > 0) /* wait nonblock, gid == 1 */
        ;
}

/*
 * server program illustrating command writing directly to socket
 */
void serve(int sockfd)
{
    int clfd;
    pid_t pid;

    if (signal_rest(SIGCHLD, sig_chld) == SIG_ERR)
        err_sys("signal_rest error");

    for (;;) {
        clfd = accept(sockfd, NULL, NULL);
        if (clfd < 0) {
            syslog(LOG_ERR, "uptime_server: accept error: %s", 
                    strerror(errno));
            exit(1);
        }
        if ((pid = fork()) < 0) {
            syslog(LOG_ERR, "uptime_server: fork error: %s",
                    strerror(errno));
        } else if (pid == 0) { /* child */
            /*
             * the daemonize already open the STDIN_FILENO, STDOUT_FILENO,
             * STDERR_FILENO to /dev/null
             */
            if (dup2(clfd, STDOUT_FILENO) != STDOUT_FILENO ||
                    dup2(clfd, STDERR_FILENO) != STDERR_FILENO) {
                syslog(LOG_ERR, "uptime_server: dup2 error");
                exit(1);
            }
            close(clfd);
            execl("usr/bin/uptime", "uptime", (char *)0);
            syslog(LOG_ERR, "uptime_server: unexpected return from exec: %s",
                    strerror(errno));
        } else { /* parent */
            close(clfd);
        }
        sleep(100);
    }
}

int main(int argc, char **argv)
{
    struct addrinfo *ailist, *aip;
    struct addrinfo hint;
    int sockfd, err, n;
    char *host;

    if (argc != 1)
        err_quit("%s", *argv);
#ifdef _SC_HOST_NAME_MAX
    n = sysconf(_SC_HOST_NAME_MAX);
    if (n < 0) /* best guess */
#endif
        n = HOST_NAME_MAX;
    host = malloc(n);
    if (host == NULL)
        err_sys("malloc error");
    if (gethostname(host, n) < 0)
        err_sys("gethostname error");
    daemonize("uptime_server");
    hint.ai_flags = AI_CANONNAME;
    hint.ai_family = 0;
    hint.ai_socktype = SOCK_STREAM;
    hint.ai_protocol = 0;
    hint.ai_addrlen = 0;
    hint.ai_canonname = NULL;
    hint.ai_addr = NULL;
    hint.ai_next = NULL;
    if ((err = getaddrinfo(host, "ruptime", &hint, &ailist)) != 0) {
        syslog(LOG_ERR, "ruptime getaddrinfo error: %s", gai_strerror(err));
        exit(1);
    }
    for (aip = ailist; aip != NULL; aip = aip->ai_next) {
        if ((sockfd = initserver(SOCK_STREAM, aip->ai_addr, 
                        aip->ai_addrlen, QLEN)) >= 0) {
            serve(sockfd);
            exit(0);
        }
    }
    exit(1);
}
