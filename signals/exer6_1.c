#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include "myapue.h"

#define MAX_COUNT 1000000

static void wrcount(char *);

int main(void)
{
    int fd;
    pid_t pid;

    TELL_WAIT();

    fd = open("./exer6_count.txt", O_RDWR|O_APPEND);
    if (fd < 0)
        return -1;
    ftruncate(fd, 0);
    if (write(fd, "0\n", 2) != 2) /* write 0 to file */
        return -1;
    if (close(fd) < 0)
        return -1;

    if ((pid = fork()) < 0) {
        return -1;
    } else if (pid == 0) {
        WAIT_PARENT();
        wrcount("child added");
        TELL_PARENT(getppid()); /* tell parent we're done */
    } else {
        wrcount("parent added");
        TELL_CHILD(pid);
        WAIT_CHILD(); /* wait for child to finish */
    }
    return 0;
}

static void wrcount(char *str)
{
    char rbuf[16];
    char wbuf[16];
    size_t len;
    ssize_t n;
    long int count;
    int fd;


    fd = open("./exer6_count.txt", O_RDWR|O_APPEND);
    if (fd < 0)
        exit(1);

    n = read(fd, rbuf, 16);
    if (n < 0)
        exit(1);

    count = atol(rbuf);
    if (count > MAX_COUNT)
        return; /* do nothing */
    count++; 

    sprintf(wbuf, "%ld %s\n", count, str); 
    len = strlen(wbuf) + 1;
    if ((n = write(fd, wbuf, len)) < 0)
        exit(1);
    if (n != len) 
        exit(1);

    if (close(fd) < 0)
        exit(1);

    return;
}
