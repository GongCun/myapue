#include "myapue.h"
#include <unistd.h>
#include <sys/resource.h>

int main(void)
{
    struct rlimit rl;

    if (getrlimit(RLIMIT_NOFILE, &rl) < 0)
        err_sys("getrlimit error");
    printf("_SC_OPEN_MAX = %ld\n", (unsigned long)sysconf(_SC_OPEN_MAX));
    printf("rlim_max = %ld\n", (unsigned long)rl.rlim_max);
    printf("rlim_cur = %ld\n", (unsigned long)rl.rlim_cur);
    printf("RLIM_INFINITY = %ld\n", (unsigned long)RLIM_INFINITY);

    exit(0);
}
