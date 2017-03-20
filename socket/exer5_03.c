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

/*
 * server program illustrating command writing directly to socket
 */
void serve(int sockfd)
{
    int clfd, n;
    char buff[MAXLINE];
    pid_t pid;

    clfd = accept(sockfd, NULL, NULL); 
    if (clfd < 0)
        err_sys("accept error"); 
    if ((pid = fork()) < 0) { 
        err_sys("fork error"); 
    } else if (pid == 0) { /* child */
        while ((n = read(clfd, buff, sizeof(buff)-1)) != 0) {
            if (n < 0)
                err_sys("read error");
            buff[n] = 0;
            printf("read %d bytes %s\n", n, buff);
        }
        printf("read EOF\n");
        exit(0);
    } else { /* parent */
        close(clfd);
        waitpid(pid, NULL, 0);
    }
    return;
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

    sockfd = Tcp_listen(host, "exer5serv", NULL);
    serve(sockfd);
    exit(0);
}
