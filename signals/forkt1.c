#include <sys/wait.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>

static void sig_int(int), sig_quit(int), sig_chld(int);

int main(int argc, char **argv)
{
    pid_t pid;
    int status;
    struct sigaction ignore, saveinr, savequit;
    sigset_t chldmask, savemask;

    /*
    if (argc != 2) {
        printf("missing argument\n");
        return 1;
    }
    */

    printf("parent pid: %ld\n", (long)getpid());

    /* set the current handler */
    if (signal(SIGINT, sig_int) == SIG_ERR)
        return -1;
    if (signal(SIGQUIT, sig_quit) == SIG_ERR)
        return -1;
    if (signal(SIGCHLD, sig_chld) == SIG_ERR)
        return -1;

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
        printf("child pid: %ld\n", (long)getpid());
        /* restore previous signal action & reset signal mask */
        sigaction(SIGINT, &saveinr, NULL);
        sigaction(SIGQUIT, &savequit, NULL);
        sigprocmask(SIG_SETMASK, &savemask, NULL);

        /* execl("/bin/bash", "bash", "-c", argv[1], (char *)0); */
        sleep(10);
        /* _exit(127); execl error, shouldn't get here. */
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

    /* raise(SIGINT); raise(SIGQUIT); raise(SIGCHLD); */

    printf("resume parent process\n");
    sleep(30);
    return status;
}

static void sig_int(int signo)
{
    printf("caught SIGINT\n");
}

static void sig_quit(int signo)
{
    printf("caught SIGQUIT\n");
}

static void sig_chld(int signo)
{
    printf("caught SIGCHLD\n");
}
