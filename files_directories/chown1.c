#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(void)
{
    long res;

    errno = 0;
    /*
     * the following "pathconf" not take effect, but
     * # getconf _POSIX_CHOWN_RESTRICTED chown.file
     * # 200112
     */
    if ((res = pathconf("chown.file", _POSIX_CHOWN_RESTRICTED)) < 0 && errno == 0)
    {
        printf("chown no restricted\n");
    } else if (errno != 0) {
        perror("pathconf");
        exit(1);
    } else {
        printf("restricted: %ld\n", res);
    }
    exit(0);
}
