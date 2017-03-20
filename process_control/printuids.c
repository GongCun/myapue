#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(void)
{
    printf("real uid = %d, effective uid = %d\n", getuid(), geteuid());

    return 0;
}
