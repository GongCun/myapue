#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>

sigset_t mask;

void *thr_fn(void *arg)
{
    int err, signo;

    for (;;) {
        err = sigwait(&mask, &signo);
        if (err != 0)
            return (void *)1;
        if (signo == SIGALRM) {
            printf("caugh SIGALRM\n");
            return (void *)0;
        }
    }
    return (void *)0;
}

int main(void)
{
    int err;
    sigset_t oldmask;
    pthread_t tid;

    sigemptyset(&mask);
    sigaddset(&mask, SIGALRM);
    if ((err = pthread_sigmask(SIG_BLOCK, &mask, &oldmask)) != 0) {
        printf("%s\n", strerror(err));
        return 1;
    }

    alarm(5);

    err = pthread_create(&tid, NULL, thr_fn, 0);
    if (err != 0) {
        printf("%s\n", strerror(err));
        return 1;
    }

    err = pthread_join(tid, NULL);
    if (err != 0) {
        printf("%s\n", strerror(err));
        return 1;
    }

    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0) {
        perror("SIG_SETMASK error");
        return 1;
    }
    return 0;
}

