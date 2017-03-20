#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(void)
{
    long pathmax;

    errno = 0;
    if ((pathmax = pathconf("/", _PC_PATH_MAX)) < 0 && errno == 0)
    {
        printf("pathmax is %ld\n", pathmax);
    } else if (errno != 0) {
        perror("pathconf");
        exit(1);
    } else {
        printf("pathmax is %ld\n", pathmax);
    }
    exit(0);
}
