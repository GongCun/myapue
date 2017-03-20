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
            strcpy(environ[i], envbuf);
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
    pthread_t tid1, tid2, tid3, tid4, tid5;
    pthread_attr_t attr;

    printf("sizeof0 = %ld\n", sizeof(environ[0]));
    printf("sizeof1 = %ld\n", sizeof(environ[1]));
    printf("sizeof2 = %ld\n", sizeof(environ[2]));
    printf("sizeof3 = %ld\n", sizeof(environ[3]));
    printf("sizeof4 = %ld\n", sizeof(environ[4]));
    printf("strlen0 = %ld\n", strlen(environ[0]));
    printf("strlen1 = %ld\n", strlen(environ[1]));
    printf("strlen2 = %ld\n", strlen(environ[2]));
    printf("strlen3 = %ld\n", strlen(environ[3]));
    printf("strlen4 = %ld\n", strlen(environ[4]));

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_create(&tid1, &attr, thd, (void *)"TEST1=a");
    pthread_create(&tid2, &attr, thd, (void *)"TEST2=b");
    pthread_create(&tid3, &attr, thd, (void *)"TEST3=c");
    pthread_create(&tid4, &attr, thd, (void *)"TEST4=d");
    pthread_create(&tid5, &attr, thd, (void *)"TEST5=e");

    sleep(5);
    pthread_attr_destroy(&attr);
    pthread_mutex_destroy(&env_mutex);
    printf("TEST1 = %s\n", getenv("TEST1"));
    printf("TEST2 = %s\n", getenv("TEST2"));
    printf("TEST3 = %s\n", getenv("TEST3"));
    printf("TEST4 = %s\n", getenv("TEST4"));
    printf("TEST5 = %s\n", getenv("TEST5"));

    return 0;
}
