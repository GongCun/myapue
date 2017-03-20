#include "myapue.h"
#include <termios.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <sys/time.h>
#include <poll.h>

static void sig_term(int signo);
static void sig_winch(int signo);

int fdm;

int main(void)
{
    int maxfd, nread;
    pid_t pid;
    char slave_name[20], buf[MAXLINE];
    struct termios orig_termios;
    struct winsize size;
    struct pollfd fds[2];

    if (isatty(STDIN_FILENO)) {
        if (tcgetattr(STDIN_FILENO, &orig_termios) < 0)
            err_sys("tcgetattr error on stdin");
        if (ioctl(STDIN_FILENO, TIOCGWINSZ, (char *)&size) < 0)
            err_sys("TIOCGWINSZ error");
        pid = pty_fork(&fdm, slave_name, sizeof(slave_name),
                &orig_termios, &size);
    } else {
        pid = pty_fork(&fdm, slave_name, sizeof(slave_name),
                NULL, NULL);
    }

    if (pid < 0) {
        err_sys("pty_fork error");
    } else if (pid == 0) { /* child */
        if (execlp("./sub_prog", "sub_prog", (char *)0) < 0)
            err_sys("can't execute sub_prog");
    }

    /* parent continue... */
    if (signal(SIGTERM, sig_term) == SIG_ERR)
        err_sys("signal(SIGTERM) error");
    if (signal(SIGWINCH, sig_winch) == SIG_ERR)
        err_sys("signal(SIGWINCH) error");

    if (isatty(STDIN_FILENO)) {
        if (tty_raw(STDIN_FILENO) < 0)
            err_sys("tty_raw error");
        if (atexit(tty_atexit) < 0)
            err_sys("atexit error");
    }

    fds[0].fd = STDIN_FILENO;
    fds[0].events = POLLIN;
    fds[1].fd = fdm;
    fds[1].events = POLLIN;
    for (;;) {
        if (poll(fds, 2, -1) < 0) {
            if (errno == EINTR)
                continue;
            err_sys("poll error");
        }

        if (fds[0].revents & POLLIN) {
            if ((nread = read(STDIN_FILENO, buf, sizeof(buf))) < 0)
                err_sys("read error from stdin");
            else if (nread == 0) {
                break;
            } else {
                if (writen(fdm, buf, nread) != nread)
                    err_sys("writen error on master pty");
            }
        }
        if (fds[1].revents & POLLIN) {
            if ((nread = read(fdm, buf, sizeof(buf))) <= 0)
                break; /* error, or EOF */
            if (writen(STDOUT_FILENO, buf, nread) != nread)
                err_sys("writen error to stdout");
        }
    }
    exit(0);
}

static void sig_term(int signo)
{
    if (ioctl(fdm, TIOCSIG, (char *)SIGTERM) < 0)
        err_sys("TIOCSIG error");
}

static void sig_winch(int signo)
{
    struct winsize size;

    if (ioctl(STDIN_FILENO, TIOCGWINSZ, (char *)&size) < 0)
        err_sys("TIOCGWINSZ error");
    if (ioctl(fdm, TIOCSWINSZ, &size) < 0)
        err_sys("TIOCSWINSZ error");
}
