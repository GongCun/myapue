#include "ystem.h"

int ystem(const char *cmdstring)
{
    pid_t pid;
    int status;
    struct sigaction ignore, saveinr, savequit;
    sigset_t chldmask, savemask;

    if (cmdstring == NULL)
        return 1;

    ignore.sa_handler = SIG_IGN;
    sigemptyset(&ignore.sa_mask);
    ignore.sa_flags = 0;

    if (sigaction(SIGINT, &ignore, &saveinr) < 0)
        return -1;
    if (sigaction(SIGQUIT, &ignore, &savequit) < 0)
        return -1;
    sigemptyset(&chldmask); /* now block SIGCHLD */
    sigaddset(&chldmask, SIGCHLD);
    if (sigprocmask(SIG_BLOCK, &chldmask, &savemask) < 0)
        return -1;

    if ((pid = fork()) < 0) {
        status = -1;
    } else if (pid == 0) {
        /* restore previous signal action & reset signal mask */
        sigaction(SIGINT, &saveinr, NULL);
        sigaction(SIGQUIT, &savequit, NULL);
        sigprocmask(SIG_SETMASK, &savemask, NULL);

        execl("/bin/bash", "bash", "-c", cmdstring, (char *)0);
        _exit(127); /* execl error, shouldn't get here. */
    } else {
        while(waitpid(pid, &status, 0) < 0)
            if (errno != EINTR) {
                status = -1;
                break;
            }
    }

    /* restore previous signal action & reset signal mask */
    if (sigaction(SIGINT, &saveinr, NULL) < 0)
        return -1;
    if (sigaction(SIGQUIT, &savequit, NULL) < 0)
        return -1;
    if (sigprocmask(SIG_SETMASK, &savemask, NULL) < 0)
        return -1;

    return status;
}

