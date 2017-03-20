#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>

#define MAXSTRLEN 2048
extern char **environ;
char ***envlist;

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
    int i, j;
    int len;
    char *buf;

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
            strcpy(environ[i], envbuf);
            return 0;
        }
    }
    if (environ[i] == NULL) {
        buf = malloc(strlen(envbuf) + 1);
        strcpy(buf, envbuf);
        envlist = malloc(sizeof(char **) * (i+1));
        for (j = 0; j < i; j++) {
            envlist[j] = &environ[j];
        }
        envlist[i] = &buf;
        for (j = 0; j <= i; j++) {
            environ[j] = *envlist[j];
        }
        environ[i+1] = NULL;
        printf("envlist = %s\n", (*envlist[i]));
        printf("environ = %s\n", environ[i]);
        return 0;
    }
    return 0;
}

int main(void)
{
    char envs[5][MAXSTRLEN];
    strcpy(envs[0],"TEST1=a");
    strcpy(envs[1],"TEST2=b");
    strcpy(envs[2],"TEST3=c");
    strcpy(envs[3],"TEST4=d");
    strcpy(envs[4],"TEST5=e");
    putenv_r(envs[0]);
    putenv_r(envs[1]);
    putenv_r(envs[2]);
    putenv_r(envs[3]);
    putenv_r(envs[4]);
    putenv_r("TEST6=f");
    putenv_r("TEST7=g");
    putenv_r("TEST8=h");
    putenv_r("TEST9=i");
    printf("SHELL = %s\n", getenv("SHELL"));
    printf("TEST1 = %s\n", getenv("TEST1"));
    printf("TEST2 = %s\n", getenv("TEST2"));
    printf("TEST3 = %s\n", getenv("TEST3"));
    printf("TEST4 = %s\n", getenv("TEST4"));
    printf("TEST5 = %s\n", getenv("TEST5"));
    printf("TEST6 = %s\n", getenv("TEST6"));
    printf("TEST7 = %s\n", getenv("TEST7"));
    printf("TEST8 = %s\n", getenv("TEST8"));
    printf("TEST9 = %s\n", getenv("TEST9"));

    return 0;
}
