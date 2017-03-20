#include "myapue.h"

int main(void)
{
    char *ptr;
    printf("fd 0: %s\n", (ptr = ttyname(0)) ? ptr : "not a tty");
    printf("fd 1: %s\n", (ptr = ttyname(1)) ? ptr : "not a tty");
    printf("fd 2: %s\n", (ptr = ttyname(2)) ? ptr : "not a tty");
    exit(0);
}
