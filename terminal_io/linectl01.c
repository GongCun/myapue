#include "myapue.h"
#include <termios.h>
#include <signal.h>

int main(void)
{ 
    int c = getc(stdin);
    if (tcflow(STDIN_FILENO, TCOOFF) != 0)
        err_sys("tcflow (off) error");

    printf("character = %c\n", (char)c);

    sleep(5);
    if (tcflow(STDIN_FILENO, TCOON) != 0)
        err_sys("tcflow (on) error");
    exit(0);
}

