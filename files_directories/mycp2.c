#include <sys/types.h>
#include <sys/uio.h>
#include "myapue.h"

#define RWRWRW (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)
int main(int argc, char **argv)
{
    int n, fd1, fd2;
    char buffer[1];

    if (argc != 3)
        err_quit("mycp <source_file> <target_file>");

    if ((fd1 = open(argv[1], O_RDONLY)) < 0)
        err_quit("open %s error", argv[1]);
    if ((fd2 = open(argv[2], O_WRONLY|O_TRUNC|O_CREAT, RWRWRW)) < 0)
        err_quit("open %s error", argv[2]);

    while((n = read(fd1, buffer, 1)) > 0) {
        if (buffer[1] == '\0')
            continue;
        errno = 0;
        if (write(fd2, buffer, n) != n) {
            if (errno != 0)
                err_sys("write error");
            else {
                printf("write error\n");
                exit(1);
            }
        }
    }

    exit(0);
}
