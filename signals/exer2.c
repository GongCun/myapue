#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

#define SIG2STR_MAX 1024

int sig2str(int, char *);

int main(void)
{
    char buf[SIG2STR_MAX];
    int status;

    if ((status = sig2str(SIGHUP, buf)) == 0)
        printf("%s\n", buf);
    if ((status = sig2str(SIGINT, buf)) == 0)
        printf("%s\n", buf);
    if((status = sig2str(SIGQUIT, buf)) == 0)
        printf("%s\n", buf);
    if ((status = sig2str(SIGCHLD, buf)) == 0)
        printf("%s\n", buf);

    return 0;
}

int sig2str(int signo, char *str)
{
    int status;

    status = 0;
    switch(signo) {
        case SIGHUP:
            strcpy(str, "HUP");
            break;
        case SIGINT:
            strcpy(str, "INT");
            break; 
        case SIGQUIT:
            strcpy(str, "QUIT");
            break;
        /* remaining signals can be here */
        default:
            status = -1;
            break;
    }
    return status;
}
