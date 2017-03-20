#include "myapue.h"
#include <termios.h>
#include <signal.h>


static void sig_alrm(int signo)
{
    if (tcflow(STDIN_FILENO, TCOON) != 0)
        err_sys("tcflow (on) error");
}

int main(void)
{ 
    if (signal(SIGALRM, sig_alrm) == SIG_ERR)
        err_sys("signal error");
    alarm(5);

    int c = getc(stdin);
    if (tcflow(STDIN_FILENO, TCOOFF) != 0)
        err_sys("tcflow (off) error");
    printf("character = %c\n", (char)c);
    pause();
    alarm(0);

    exit(0);
}

