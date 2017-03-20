#include <signal.h>
#include <errno.h>

/*
 * <signal.h> usually defines NSIG to include signal number 0
 */
#define SIGBAD(signo) ((signo) <= 0 || (signo) >= NSIG)

int sigaddset1(sigset_t *set, int signo)
{
    if (SIGBAD(signo)) {
        errno = EINVAL;
        return -1;
    }
    *set |= 1 << (signo - 1); /* turn bit on */
    return 0;
}

int sigismember1(const sigset_t *set, int signo)
{
    if (SIGBAD(signo)) {
        errno = EINVAL;
        return -1;
    }
    return (*set & (1 << (signo - 1))) != 0;
}

int sigdelset1(sigset_t *set, int signo)
{
    if (SIGBAD(signo)) {
        errno = EINVAL;
        return -1;
    }
    *set &= ~(1 << (signo -1)); /* turn bit off */
    return 0;
}
