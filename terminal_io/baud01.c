#include "myapue.h"
#include <termios.h>

static void getspeed(struct termios);

int main(void)
{
    struct termios term;

    if (tcgetattr(STDIN_FILENO, &term) < 0)
        err_sys("tcgetattr error");
    getspeed(term);

    if (cfsetispeed(&term, (speed_t)38400) != 0)
        err_sys("cfsetispeed in-speed error");
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &term) < 0)
        err_sys("tcsetattr in-speed error");

    if (tcgetattr(STDIN_FILENO, &term) < 0)
        err_sys("tcgetattr error");
    getspeed(term);
    exit(0);
}

static void getspeed(struct termios term)
{
    speed_t ispeed, ospeed;

    ispeed = cfgetispeed(&term);
    ospeed = cfgetospeed(&term);
    printf("in speed = %d\nout speed = %d\n",
            (int)ispeed, (int)ospeed);
    return;
}
    
