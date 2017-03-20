#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>

#define MAXSTRLEN 2048
extern char **environ;

pthread_mutex_t env_mutex;
static pthread_once_t init_done = PTHREAD_ONCE_INIT;

static void pthread_init(void)
{
    pthread_mutexattr_t attr;

    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&env_mutex, &attr);
    pthread_mutexattr_destroy(&attr);
}

int putenv_r(char *envbuf)
{
    int i;
    int len;

    pthread_once(&init_done, pthread_init);
    /* 
     * envbuf format: name=value 
     * get the "name"
     */
    for (i = 0; envbuf[i] != '\0'; i++) {
        if (envbuf[i] == '=')
            break;
    }
    if (envbuf[i] == '\0') {
        return 1;
    } 

    pthread_mutex_lock(&env_mutex);
    len = i;
    for (i = 0; environ[i] != NULL; i++) { 
        if ((strncmp(envbuf, environ[i], len) == 0) &&
                   (environ[i][len] == '='))  /* have the same name */
        { 
            /* have the same value */
            if (strcmp(&envbuf[len+1], &environ[i][len+1]) == 0) { 
                return 0; 
            }
            /* have the different value */ 
            environ[i] = envbuf;
            return 0;
        }
    }
    if (environ[i] == NULL) {
        environ[i] = envbuf;
        environ[i+1] = NULL;
    }
    pthread_mutex_unlock(&env_mutex);
    return 0;
}

void *thd(void *arg)
{
    putenv_r((char *)arg);
    pthread_exit((void *)0);
}

int main(void)
{
    pthread_t tid1, tid2, tid3, tid4;
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_create(&tid1, &attr, thd, (void *)"SHELL=/bin/sh");
    pthread_create(&tid2, &attr, thd, (void *)"SHELL=/bin/bash");
    pthread_create(&tid3, &attr, thd, (void *)"SHELL=/usr/bin/rsh");
    pthread_create(&tid4, &attr, thd, (void *)"SHELL=/bin/ksh");

    sleep(2);
    pthread_attr_destroy(&attr);
    pthread_mutex_destroy(&env_mutex);
    printf("SHELL = %s\n", getenv("SHELL"));

    return 0;
}
