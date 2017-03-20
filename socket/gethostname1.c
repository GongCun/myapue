#include "myapue.h"
#include <limits.h>

int main(void)
{
    long n;
    char *host;
    n = sysconf(_SC_HOST_NAME_MAX);
    if ((host = malloc(n)) == NULL)
        err_sys("malloc error");
    if (gethostname(host, n) < 0)
        err_sys("gethostname error");
    printf("host = %s\n", host);
    exit(0);
}
