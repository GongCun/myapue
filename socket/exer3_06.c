#include "myapue.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/ioctl.h>

#define SERV "ruptime"

static void serve(int *, int);

int main(void)
{
    int n;
    char *host;
    int listenfd[2];

    n = (int)sysconf(_SC_HOST_NAME_MAX);
    if (n < 0)
        err_sys("sysconf error");
    if ((host = malloc(n)) == NULL)
        err_sys("malloc error");
    if (gethostname(host, n) < 0)
        err_sys("gethostname error");

    listenfd[0] = Tcp_listen(host, SERV, NULL);
    listenfd[1] = Tcp_listen("localhost", SERV, NULL);
    
    serve(listenfd, 2);
    exit(0);
}

static void serve(int sockfd[], int len)
{
    int i, maxfd = 0, clfd;
    char buff[100]; 
    FILE *fp;
    fd_set rset;

    FD_ZERO(&rset);

    for (;;) { 
        for (i = 0; i < len; i++) { 
            FD_SET(sockfd[i], &rset); 
            maxfd = maxfd > sockfd[i] ? maxfd : sockfd[i];
        }
        if (select(maxfd+1, &rset, NULL, NULL, NULL) < 0) /* don't wait at all */
            err_sys("select error");
        for (i = 0; i < len; i++) { 
            if (FD_ISSET(sockfd[i], &rset)) {
                clfd = accept(sockfd[i], NULL, NULL);
                if ((fp = popen("/usr/bin/uptime", "r")) == NULL) {
                    sprintf(buff, "error: %s\n", strerror(errno));
                    send(clfd, buff, strlen(buff), 0);
                } else {
                    while (fgets(buff, sizeof(buff), fp) != NULL)
                        send(clfd, buff, strlen(buff), 0);
                    pclose(fp);
                }
                close(clfd); 
            }
        } 
    }
}
