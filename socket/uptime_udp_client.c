#include "myapue.h"
#include <errno.h>
#include <netdb.h>
#include <sys/socket.h>
#include <signal.h>

#define TIMEOUT 20
#define BUFLEN 128

static void sig_alrm(int signo)
{
    return;
}

void print_uptime(int sockfd, struct addrinfo *aip)
{
    int n;
    char buf[BUFLEN];

    buf[0] = 0;
    if (sendto(sockfd, buf, 1, 0, 
                aip->ai_addr, aip->ai_addrlen) < 0)
        err_sys("sendto error");
    alarm(TIMEOUT);
    if ((n = recvfrom(sockfd, buf, BUFLEN, 0, NULL, NULL)) < 0) {
        if (errno != EINTR)
            alarm(0);
        err_sys("recvfrom error");
    }
    alarm(0);
    write(STDOUT_FILENO, buf, n);
}

int main(void)
{
    struct addrinfo *ailist, *aip;
    struct addrinfo hint;
    int sockfd, err, n;
    char *host;
    struct sigaction sa;

    n = (int)sysconf(_SC_HOST_NAME_MAX);
    if ((host = malloc(n)) == NULL)
        err_sys("malloc error");
    if (gethostname(host, n) < 0)
        err_sys("gethostname error");
    sa.sa_handler = sig_alrm;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGALRM, &sa, NULL) < 0)
        err_sys("sigaction error");
    hint.ai_flags = 0;
    hint.ai_family = 0;
    hint.ai_socktype = SOCK_DGRAM; /* !! */
    hint.ai_protocol = 0;
    hint.ai_addrlen = 0;
    hint.ai_canonname = NULL;
    hint.ai_addr = NULL;
    hint.ai_next = NULL;
    if ((err = getaddrinfo(host, "ruptime", &hint, &ailist)) != 0)
        err_quit("getaddrinfo error: %s", gai_strerror(err));
    for (aip = ailist; aip != NULL; aip = aip->ai_next) {
        if ((sockfd = socket(aip->ai_family, SOCK_DGRAM, 0)) < 0) {
            err = errno;
        } else {
            print_uptime(sockfd, aip);
            exit(0);
        }
    }
    fprintf(stderr, "can't connect to %s: %s\n", host,
            strerror(err));
    exit(1);
}

