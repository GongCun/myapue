#include <limits.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define MAXSTRINGSZ 4096

extern char **environ;

static char envbuf[MAXSTRINGSZ];

char *getenv1(const char *name)
{
    int i, len;
    
    len = strlen(name);
    for (i = 0; environ[i] != NULL; i++) {
        if ((strncmp(name, environ[i], len) == 0) &&
                environ[i][len] == '=') {
            strcpy(envbuf, &environ[i][len+1]);
            return envbuf;
        }
    }
    return NULL;
}

void *thd(void *arg)
{
    printf("SHELL = %s\n", getenv1("SHELL"));
    printf("TERM_PROGRAM = %s\n", getenv1("TERM_PROGRAM"));
    printf("MANPATH = %s\n", getenv1("MANPATH"));
    printf("TERM = %s\n", getenv1("TERM"));
    printf("PWD = %s\n", getenv1("PWD"));
    pthread_exit((void *)0);
}


int main(void)
{
    int err;
    pthread_t tid1, tid2;
    pthread_attr_t attr;

    err = pthread_attr_init(&attr);
    if (err != 0)
        return err;
    err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    if (err != 0)
        return err;
    err = pthread_create(&tid1, &attr, thd, NULL);
    if (err != 0)
        return err;
    err = pthread_create(&tid2, &attr, thd, NULL);
    if (err != 0)
        return err;
    pthread_attr_destroy(&attr);
    sleep(1);
    return 0;
}
