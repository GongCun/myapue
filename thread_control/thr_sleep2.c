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
            printf("%s caugh SIGALRM\n", (char *)arg);
            return (void *)0;
        }
    }
    return (void *)0;
}

int main(void)
{
    int err;
    sigset_t oldmask;
    pthread_t tid1, tid2;

    sigemptyset(&mask);
    sigaddset(&mask, SIGALRM);
    if ((err = pthread_sigmask(SIG_BLOCK, &mask, &oldmask)) != 0) {
        printf("%s\n", strerror(err));
        return 1;
    }

    alarm(5);
    alarm(8);

    err = pthread_create(&tid1, NULL, thr_fn, "tid1");
    if (err != 0) {
        printf("%s\n", strerror(err));
        return 1;
    }

    err = pthread_create(&tid2, NULL, thr_fn, "tid2");
    if (err != 0) {
        printf("%s\n", strerror(err));
        return 1;
    }

    sleep(10);

    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0) {
        perror("SIG_SETMASK error");
        return 1;
    }

    return 0;
}

