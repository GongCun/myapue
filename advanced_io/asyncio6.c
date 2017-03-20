#include "myapue.h"
#include <signal.h>
#include <sys/time.h>
#include <aio.h>

static void sig_io(int);
struct aiocb aio_cb;

int main(void)
{
    static char buf[2];
    
    /* initialize aio control block */
    aio_cb.aio_fildes = STDIN_FILENO;
    aio_cb.aio_buf = buf;
    aio_cb.aio_nbytes = 2;
    aio_cb.aio_offset = 0;
    aio_cb.aio_sigevent.sigev_notify = SIGEV_SIGNAL;
    aio_cb.aio_sigevent.sigev_signo = SIGIO;

    /* install SIGIO handler */
    if (signal(SIGIO, sig_io) == SIG_ERR)
        err_sys("signal(SIGIO) error");

    /* place a read request */
    if (aio_read(&aio_cb) < 0)
        err_sys("aio_read error");

    for (;;)
        pause();
    exit(0);
}

static void sig_io(int signo)
{
    char *buf;

    buf = (char *)aio_cb.aio_buf;
    if (aio_error(&aio_cb) != 0) {
        err_ret("aio_error");
    } else {
        /* get number of chars read */
        if (aio_return(&aio_cb) == 2) {
            buf[strlen(buf)-1] = 0;
            printf("%s\n", buf);
        } else {
            printf("aio read exception\n");
        }
    }
    /* place a new request */
    if (aio_read(&aio_cb) < 0)
        err_sys("aio_read");
    return;
}
