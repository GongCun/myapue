#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>

#define MAXSTRLEN 2048
extern char **environ;

pthread_mutex_t env_mutex;

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
            /*
            if (strlen(envbuf) <= strlen(environ[i])) {
                strcpy(environ[i], envbuf);
            } else {
                environ[i] = envbuf;
            }
            */
            environ[i] = envbuf;
            return 0;
        }
    }
    if (environ[i] == NULL)  {
        environ[i] = envbuf;
        environ[i+1] = NULL;
        return 0;
    }
    return 0;
}

int main(void)
{

    printf("/bin/bash 0x%lx\n", (unsigned long)getenv("SHELL"));
    putenv_r("SHELL=/usr/zsh");
    printf("/bin/zsh 0x%lx\n", (unsigned long)getenv("SHELL"));
    putenv_r("SHELL=/usr/bin/rsh");
    printf("/usr/bin/rsh 0x%lx\n", (unsigned long)getenv("SHELL"));
    putenv_r("SHELL=/bin/sh");
    printf("/bin/sh 0x%lx\n", (unsigned long)getenv("SHELL"));

    putenv_r("TEST1=a");
    putenv_r("TEST2=b");
    putenv_r("TEST3=c");
    putenv_r("TEST4=d");
    putenv_r("TEST5=e");
    printf("TEST1 = %s\n", getenv("TEST1"));
    printf("TEST2 = %s\n", getenv("TEST2"));
    printf("TEST3 = %s\n", getenv("TEST3"));
    printf("TEST4 = %s\n", getenv("TEST4"));
    printf("TEST5 = %s\n", getenv("TEST5"));
    return 0;
}
