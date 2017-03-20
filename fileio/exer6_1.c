#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#define OUTFILE "./outfile"

int main(void)
{
    int fd;
    int n;
    char buf[4096];

    if ((fd = open(OUTFILE, O_CREAT|O_TRUNC|O_APPEND|O_RDWR, 0666)) < 0) {
        perror("open");
        exit(1);
    }

    while ((n = read(STDIN_FILENO, buf, 4096)) > 0)
        if (write(fd, buf, n) != n) {
            printf("write error\n");
            exit(1);
        }
    if (n < 0) {
        perror("read");
        exit(1);
    }


    if (lseek(fd, -10, SEEK_END) < 0) {
        perror("lseek");
        exit(1);
    }

    sprintf(buf, " ");
    printf("buf is %s\n", buf);
    if ((n = read(fd, buf, 10)) != 10) {
        if (n < 0) {
            perror("lseek read");
            exit(1);
        }
        printf("lseek read error\n");
        exit(1);
    }
    printf("read content is: %s\n", buf);

    if (lseek(fd, 5, SEEK_SET) < 0) {
        perror("lseek");
        exit(1);
    }
    sprintf(buf, "lseek write on O_APPEND mode");
    if ((n = write(fd, buf, strlen(buf)+1)) < strlen(buf)+1) {
        if (n < 0) {
            perror("lseek write");
            exit(1);
        }
        printf("lseek write error\n");
        exit(1);
    }
    return 0;
}
    
