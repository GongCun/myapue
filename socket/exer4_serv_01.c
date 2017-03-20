#include "myapue.h"
#include <netdb.h>
#include <errno.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <limits.h>

#define BUFLEN 128
#define QLEN 10
#define SERV "psserv"

#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX 256
#endif

void serve(int sockfd)
{
    int clfd;
    FILE *fp;
    char buf[BUFLEN];

    for (;;) {
        clfd = accept(sockfd, NULL, NULL);
        if (clfd < 0) {
            syslog(LOG_ERR, "ps_server: accept error: %s", 
                    strerror(errno));
            exit(1);
        }
        if ((fp = popen("ps", "r")) == NULL) {
            sprintf(buf, "error: %s\n", strerror(errno));
            send(clfd, buf, strlen(buf), 0);
        } else {
            while (fgets(buf, BUFLEN, fp) != NULL)
                send(clfd, buf, strlen(buf), 0);
            pclose(fp);
        }
        close(clfd);
    }
}

int main(int argc, char **argv)
{
    int sockfd, n;
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
    daemonize("ps_server"); 
    sockfd = Tcp_listen(host, SERV, NULL);
    serve(sockfd);
    exit(0);
}
