#include "myapue.h"

int main(void)
{
    int i;
    char c;

    if (tty_raw(STDIN_FILENO) < 0)
        err_sys("tty_raw error");
    printf("Enter raw mode characters, terminate with DELETE\n");

    while ((i = read(STDIN_FILENO, &c, 1)) == 1) {
        if ((c &= 255) == 0177)
            break;
        printf("%o\n", c);
    }

    exit(0);
}
