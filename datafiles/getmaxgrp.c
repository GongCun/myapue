#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    printf("NGROUPS_MAX: %lu\n", sysconf(_SC_NGROUPS_MAX));
    exit(0);
}
