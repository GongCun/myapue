#include "myapue.h"

int main(void)
{
    int fd, i;
    pid_t pid;
    off_t currpos;

    TELL_WAIT(); 

    fd = open("temp.file", O_RDWR|O_CREAT, FILE_MODE); 
    if (fd < 0) 
        err_sys("open error");

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid > 0) { /* parent */
        for (i = 0; i < 10; i++) {
            currpos = lseek(fd, 0, SEEK_CUR);
            printf("parent get current position: %lld\n", currpos);
            TELL_CHILD(pid);
            WAIT_CHILD();
        }
    } else { /* child */
        for (i = 0; i < 10; i++) { 
            WAIT_PARENT(); 
            lseek(fd, i, SEEK_SET); 
            TELL_PARENT(getppid());
        }
    }
    exit(0);
}
