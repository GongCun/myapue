#include <poll.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define TIMEOUT 5 /* in seconds */
int main(void)
{
    struct pollfd fds[2];
    int ret;

    fds[0].fd = STDIN_FILENO;
    fds[0].events = POLLIN;

    /* watch stdout for ability to write (almost always true) */
    fds[1].fd = STDOUT_FILENO;
    fds[1].events = POLLOUT;

    ret = poll(fds, 2, TIMEOUT * 1000);
    if (ret == -1) {
        perror("poll error");
        exit(1);
    }

    if (!ret) {
        printf("%d seconds elapsed.\n", TIMEOUT);
        exit(0);
    }
    fflush(NULL);
    printf("%d\n", fds[0].revents);
    printf("%d\n", fds[1].revents);
    printf("%d\n", POLLIN);
    printf("%d\n", POLLRDNORM);
    printf("%d\n", POLLRDBAND);
    printf("%d\n", POLLPRI);
    printf("%d\n", POLLOUT);
    printf("%d\n", POLLWRNORM);
    printf("%d\n", POLLWRBAND);
    printf("%d\n", POLLERR);
    printf("%d\n", POLLHUP);
    printf("%d\n", POLLNVAL);
    if (fds[0].revents & POLLIN)
        printf("stdin is readable\n");
    if (fds[1].revents & (POLLOUT|POLLWRBAND))
        printf("stdout is writable\n");
    exit(0);
}
