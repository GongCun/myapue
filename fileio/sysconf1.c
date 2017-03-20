#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>

int main(void)
{
    struct rlimit rl;

    if (getrlimit(RLIMIT_FSIZE, &rl) < 0) {
        perror("getrlimit failed");
        exit(1);
    }
    printf("rlim_cur = %10lu\n", (unsigned long)rl.rlim_cur);
    printf("rlim_max = %10lu\n", (unsigned long)rl.rlim_max);

    return 0;
}
