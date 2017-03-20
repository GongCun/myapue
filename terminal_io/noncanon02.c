#include "myapue.h"
#include <stdio.h>
#include <signal.h>

static void sig_catch(int);

int main(void)
{
    int i;
    char c;

    if (signal(SIGINT, sig_catch) == SIG_ERR)
        err_sys("signal(SIGINT) error");
    if (signal(SIGQUIT, sig_catch) == SIG_ERR)
        err_sys("signal (SIGQUIT) error");
    if (signal(SIGTERM, sig_catch) == SIG_ERR)
        err_sys("signal (SIGTERM) error");

    if (tty_raw(STDIN_FILENO) < 0)
        err_sys("tty_raw error");
    printf("Enter raw mode characters, terminate with DELETE\n");
    while ((i = read(STDIN_FILENO, &c, 1)) == 1) {
        if ((c &= 255) == 0177) /* 0177 = ASCII DELETE */
            break;
        /* the "UP" == 033 + 133 + 101 */
        if (c == 033) {
           if (read(STDIN_FILENO, &c, 1) != 1) /* skip '[' */
              continue; 
           if (read(STDIN_FILENO, &c, 1) != 1)
              continue; 
           switch((c &= 255)) {
               case 0101:
                   printf("up\n");
                   break;
               case 0102:
                   printf("down\n");
                   break;
               case 0103:
                   printf("right\n");
                   break;
               case 0104:
                   printf("left\n");
                   break;
               default:
                   printf("unknown key\n");
           }
        } else { 
            printf("%o\n", c); /* unsigned octal */
        }
    }
    if (tty_reset(STDIN_FILENO) < 0)
        err_sys("tty_reset error");
    if (i <= 0)
        err_sys("read error");

    if (tty_cbreak(STDIN_FILENO) < 0)
        err_sys("tty_cbreak error");
    printf("\nEnter cbreak mode characters, terminate with SIGINT\n");
    while ((i = read(STDIN_FILENO, &c, 1)) == 1) {
        c &= 255;
        printf("%o\n", c);
    }
    tty_reset(STDIN_FILENO);
    if (i <= 0)
        err_sys("read error");
    exit(0);
}

static void sig_catch(int signo)
{
    printf("signal caught\n");
    tty_reset(STDIN_FILENO);
    exit(0);
}

