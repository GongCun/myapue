#include <limits.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>

#define MAXSTRINGSZ 4096
static pthread_key_t key;
static pthread_once_t init_done = PTHREAD_ONCE_INIT;
pthread_mutex_t env_mutex = PTHREAD_MUTEX_INITIALIZER;

extern char **environ;

static void thread_init(void)
{
    pthread_key_create(&key, free);
}

char *getenv_r(const char *name)
{
    int i, len;
    char *envbuf;

    pthread_once(&init_done, thread_init);
    pthread_mutex_lock(&env_mutex);
    envbuf = (char *)pthread_getspecific(key);
    if (envbuf == NULL) {
        envbuf = malloc(MAXSTRINGSZ);
        if (envbuf == NULL) {
            pthread_mutex_unlock(&env_mutex);
            return NULL;
        }
        pthread_setspecific(key, envbuf);
    }
    len = strlen(name);
    for (i = 0; environ[i] != NULL; i++) {
        if ((strncmp(name, environ[i], len) == 0) &&
                environ[i][len] == '=') {
            strncpy(envbuf, &environ[i][len+1], MAXSTRINGSZ-1);
            pthread_mutex_unlock(&env_mutex);
            return envbuf;
        }
    }
    pthread_mutex_unlock(&env_mutex);
    return NULL;
}

void *thd(void *arg)
{
    printf("SHELL = %s\n", getenv_r("SHELL"));
    printf("TERM_PROGRAM = %s\n", getenv_r("TERM_PROGRAM"));
    printf("MANPATH = %s\n", getenv_r("MANPATH"));
    printf("TERM = %s\n", getenv_r("TERM"));
    printf("PWD = %s\n", getenv_r("PWD"));
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
    pthread_key_delete(key);
    sleep(1);
    return 0;
}
