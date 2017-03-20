#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define SOURCE ("./countfile")
#define TARGET ("./countfile.link")

int main(void) 
{
    int n;
    char buf[4096];

    if (symlink(SOURCE, TARGET) < 0) {
        perror("symlink");
    }
    if ((n = readlink(TARGET, buf, 32)) > 0)
        if (write(STDOUT_FILENO, buf, n) != n) {
            if (errno != 0)
                perror("write");
            else
                printf("write error");
            exit(1);
        }
    if (n < 0) {
        perror("readlink");
        exit(1);
    }
    putchar('\n');
    exit(0);
}
