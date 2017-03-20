#include "myapue.h"
#include <sys/socket.h>

int main(void)
{
    int fd, i;
    pid_t pid;
    off_t currpos;
    int pipe[2];

    if (s_pipe(pipe) < 0)
        err_sys("s_pipe error");

    TELL_WAIT(); 

    /*
    *fd = open("temp.file", O_RDWR|O_CREAT, FILE_MODE); 
    *if (fd < 0) 
    *    err_sys("open error");
    */

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid > 0) { /* parent */
        close(pipe[0]); 
        WAIT_CHILD();
        fd = recv_fd(pipe[1], write);
        if (fd < 0)
            err_sys("recv_fd error");
        TELL_CHILD(pid);
        for (i = 0; i < 10; i++) {
            WAIT_CHILD();
            currpos = lseek(fd, 0, SEEK_CUR);
            printf("parent get current position: %lld\n", currpos);
            TELL_CHILD(pid);
        }
    } else { /* child */
        close(pipe[1]);
        fd = open("temp.file", O_RDWR|O_CREAT, FILE_MODE);
        if (fd < 0)
            err_sys("open error");
        if (send_fd(pipe[0], fd) < 0)
            err_sys("send_fd error");
        TELL_PARENT(getppid());
        WAIT_PARENT();
        for (i = 0; i < 10; i++) { 
            lseek(fd, i, SEEK_SET); 
            TELL_PARENT(getppid());
            WAIT_PARENT(); 
        }
    }
    exit(0);
}
