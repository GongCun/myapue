#include <limits.h>
#include <string.h>
#include <stdio.h>

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

int main(void)
{
    printf("%s\n", getenv1("SHELL"));
    return 0;
}

