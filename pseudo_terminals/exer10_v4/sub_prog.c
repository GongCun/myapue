#include "myapue.h"
#include <termios.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <signal.h>

static void sig_term(int signo);
static void sig_winch(int signo);

int main(void)
{
    int n;
    char buf[MAXLINE];

    if (signal_rest(SIGTERM, sig_term) == SIG_ERR)
        err_sys("signal(SIGTERM) error");
    if (signal_rest(SIGWINCH, sig_winch) == SIG_ERR)
        err_sys("signal(SIGWINCH) error");

    if ((n = read(STDIN_FILENO, buf, sizeof(buf))) > 0) {
        printf("sub_prog: ");
        fflush(stdout);
        if (write(STDOUT_FILENO, buf, n) != n)
            err_sys("write error");
    }

    exit(0);
}

static void sig_term(int signo)
{
    printf("caught SIGTERM\n");
}


static void sig_winch(int signo)
{
    struct winsize size;

    if (ioctl(STDIN_FILENO, TIOCGWINSZ, (char *)&size) < 0)
        err_sys("TIOCGWINSZ error");
    printf("%d rows, %d columns\n", size.ws_row, size.ws_col);
}

