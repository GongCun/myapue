#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

char buf1[] = "abcdefghij";
char buf2[] = "ABCDEFGHIJ";

int main(void)
{
    int fd;

    if ((fd = creat("file.hole", S_IRUSR|S_IWUSR|S_IRGRP)) < 0)
        exit(1);
    if (write(fd, buf1, 10) != 10)
        exit(1);
    /* offset now = 10 */

    if (lseek(fd, 16384, SEEK_SET) == -1) {
        printf("seek error\n");
        exit(1);
    }
    /* offset now = 16384 */

    if (write(fd, buf2, 10) != 10)
        exit(1);
    /* offset now = 16394 */

    return 0;
}
