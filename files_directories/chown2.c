#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(void)
{
    /* run with root permission */
    if (chown("chown.file", 0, -1) < 0) {
        perror("chown error");
        exit(1);
    }
    if (lchown("chown.link", 0, -1) < 0) {
        perror("lchown error");
        exit(1);
    }
    exit(0);
}
