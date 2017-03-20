#include "myapue.h"
#include <termios.h>

int main(int argc, char **argv)
{
    int fd;
    struct termios term;

    if (argc != 2)
        err_quit("%s ttyname", *argv);

    fd = open(argv[1], O_RDWR);
    if (fd < 0)
        err_sys("open error");
    if (isatty(fd) == 0)
        err_quit("%s is not a tty", argv[1]);
    if (tcgetattr(fd, &term) < 0)
        err_sys("tcgetattr error");
    term.c_cflag |= CREAD;

    if (tcsetattr(fd, TCSAFLUSH, &term) < 0)
        err_sys("tcsetattr error");
    exit(0);
}

    
