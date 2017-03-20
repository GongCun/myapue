#include <unistd.h>
#include <stdio.h>

int main(void)
{
    printf("%ld\n", (long)getpid());
    pause();
    return 0;
}
