#include "myapue.h"
#include <termios.h>

int my_isatty(int fd)
{
    struct termios term;
    return(tcgetattr(fd, &term) != -1); /* true if no error (is a tty) */
}

int main(void)
{
    printf("fd 0: %s\n", my_isatty(0) ? "tty" : "not a tty");
    printf("fd 1: %s\n", my_isatty(1) ? "tty" : "not a tty");
    printf("fd 2: %s\n", my_isatty(2) ? "tty" : "not a tty");
    exit(0);
}
