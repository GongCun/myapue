#include <limits.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>

#define MAXSTRINGSZ 4096
extern char **environ;

pthread_mutex_t env_mutex;
static pthread_once_t init_done = PTHREAD_ONCE_INIT;

static void thread_init(void)
{
    pthread_mutexattr_t attr;

    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&env_mutex, &attr);
    pthread_mutexattr_destroy(&attr);
}

int getenv_r(const char *name, char *buf, int buflen)
{
    int i, len, olen;

    pthread_once(&init_done, thread_init);
    len = strlen(name);
    pthread_mutex_lock(&env_mutex);
    for (i = 0; environ[i] != NULL; i++) {
        if ((strncmp(name, environ[i], len) == 0) &&
                environ[i][len] == '=') {
            olen = strlen(&environ[i][len+1]);
            if (olen > buflen) {
                pthread_mutex_unlock(&env_mutex);
                return(ENOSPC);
            }
            strcpy(buf, &environ[i][len+1]);
            pthread_mutex_unlock(&env_mutex);
            return 0;
        }
    }
    pthread_mutex_unlock(&env_mutex);
    return ENOENT;
}

void *thd(void *arg)
{
    char buf[MAXSTRINGSZ];
    if (getenv_r("SHELL", buf, MAXSTRINGSZ) == 0) printf("SHELL = %s\n", buf);
    if (getenv_r("TERM_PROGRAM", buf, MAXSTRINGSZ) == 0) printf("TERM_PROGRAM = %s\n", buf);
    if (getenv_r("MANPATH", buf, MAXSTRINGSZ) == 0) printf("MANPATH = %s\n", buf);
    if (getenv_r("TERM", buf, MAXSTRINGSZ) == 0) printf("TERM = %s\n", buf);
    if (getenv_r("PWD", buf, MAXSTRINGSZ) == 0) printf("PWD = %s\n", buf);
    pthread_exit((void *)0);
}


int main(void)
{
    int err;
    pthread_t tid1, tid2;
    pthread_attr_t thd_attr;

    err = pthread_attr_init(&thd_attr);
    if (err != 0)
        return err;
    err = pthread_attr_setdetachstate(&thd_attr, PTHREAD_CREATE_DETACHED);
    if (err != 0)
        return err;
    err = pthread_create(&tid1, &thd_attr, thd, NULL);
    if (err != 0)
        return err;
    err = pthread_create(&tid2, &thd_attr, thd, NULL);
    if (err != 0)
        return err;
    pthread_attr_destroy(&thd_attr);
    sleep(1);
    return 0;
}
