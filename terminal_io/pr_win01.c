#include "myapue.h"
#include <termios.h>
#include <sys/ioctl.h>
#include <signal.h>

static void pr_winsize(int), sig_winch(int);

int main(void)
{
    if (isatty(STDIN_FILENO) == 0)
        exit(1);
    if (signal(SIGWINCH, sig_winch) == SIG_ERR)
        err_sys("signal error");
    pr_winsize(STDIN_FILENO); /* print initial size */
    for (;;)
        pause();
}

static void pr_winsize(int fd)
{
    struct winsize size;
    if (ioctl(fd, TIOCGWINSZ, (char *)&size) < 0)
        err_sys("TIOCGWINSZ error");
    printf("%d rows, %d columns\n", size.ws_row, size.ws_col);
}

static void sig_winch(int signo)
{
    printf("caught SIGWINCH\n");
    pr_winsize(STDIN_FILENO);
    return;
}
