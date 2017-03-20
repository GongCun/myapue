#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include "myapue.h"

#define MAX_COUNT 1000000
#define RECORD "./exer6_count.txt"

static void wrcount(char *);

int main(void)
{
    int fd;
    char ZERO[16];
    pid_t pid;

    TELL_WAIT();

    /* open file */
    fd = open(RECORD, O_RDWR|O_APPEND, 0);

    /*
     * handler if file not exits
     */
    if (fd < 0) { 
        /* owner has read and write permission */
        fd = open(RECORD, O_CREAT|O_TRUNC|O_RDWR, S_IRUSR|S_IWUSR);
        if (fd < 0) {
            perror("O_CREAT");
            return -1;
        } 

        sprintf(ZERO, "%8ld\n", (long)0); 
        if (write(fd, ZERO, strlen(ZERO)+1) != strlen(ZERO)+1) /* write 0 to file */
            return -1;
    }

    /* close file */
    if (close(fd) < 0)
        return -1;

    if ((pid = fork()) < 0) {
        return -1;
    } else if (pid == 0) {
        WAIT_PARENT();
        wrcount("child");
        TELL_PARENT(getppid()); /* tell parent we're done */
    } else {
        wrcount("parent");
        TELL_CHILD(pid);
        WAIT_CHILD(); /* wait for child to finish */
    }
    return 0;
}

static void wrcount(char *str)
{
    char rbuf[32];
    char wbuf[32];
    size_t len;
    ssize_t n;
    long int count;
    int fd;


    fd = open(RECORD, O_RDWR|O_APPEND);
    if (fd < 0)
        exit(1);

    if (lseek(fd, -8, SEEK_END) < 0) /* read the last value */
        exit(1);

    n = read(fd, rbuf, 8);
    if (n < 0)
        exit(1);

    count = atol(rbuf);
    if (count > MAX_COUNT) { /* don't add */
        close(fd);
        return; 
    }
    count++; 

    sprintf(wbuf, "%-10s %8ld\n", str, count);
    len = strlen(wbuf) + 1;
    if ((n = write(fd, wbuf, len)) < 0)
        exit(1);
    if (n != len) 
        exit(1);

    if (close(fd) < 0)
        exit(1);

    return;
}
