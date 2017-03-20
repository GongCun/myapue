#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#define MODE (S_IRUSR|S_IWUSR|S_IXUSR)

int main(void)
{
    if (rmdir("subdir") < 0) {
        perror("rmdir");
        exit(1);
    }
    exit(0);
}
