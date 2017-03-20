#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    pid_t pid;

    if ((pid = fork()) < 0)
        return 1;
    else if (pid == 0) {
        printf("child %ld\n", (long)getpid());
        /* exit(0); */
    }
    else
        printf("parent %ld\n", (long)getpid());

    printf("share %ld\n", (long)getpid());

    return 0;
}
