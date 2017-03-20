#include "myapue.h"
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>

#define NLOOPS 1000
#define SIZE sizeof(long) /* size of shared memory area */

#define LKF1 "temp.lock1"
#define LKF2 "temp.lock2"

static int update(long *ptr)
{
    return (*ptr)++;
}

int main(void)
{
    int i, counter;
    int fd1, fd2;
    pid_t pid;
    void *area;
    struct stat statbuf;

    if ((fd1 = open(LKF1, O_CREAT|O_EXCL, FILE_MODE)) < 0 && errno != EEXIST)
        err_sys("open %s  error", LKF1);
    if (stat(LKF2, &statbuf) < 0) {
        if (errno == ENOENT)
            unlink(LKF2);
        else 
            err_sys("stat error");
    }

    /* for redirect to a file */
    if (setvbuf(stdout, NULL, _IOLBF, 0) == EOF)
        err_sys("setvbuf error");

    if ((area = mmap(0, SIZE, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANON,
                    -1, 0)) == MAP_FAILED)
        err_sys("mmap error");

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid > 0) { /* parent */
        for (i = 1; i < NLOOPS + 1; i += 2) {
            while (1) {
                if ((fd1 = open(LKF1, O_CREAT|O_EXCL, FILE_MODE)) < 0) {
                    if (errno == EEXIST)
                        continue;
                    else
                        err_sys("open LKF1 error");
                } else {
                    break;
                }
            }
            counter = update((long *)area);
            printf("parent: i = %d; area = %ld; counter = %d\n", 
                    i, *((long *)area), counter);
            unlink(LKF2);
        } 
    } else { /* child */
        for (i = 0; i < NLOOPS; i += 2) {
            while (1) {
                if ((fd2 = open(LKF2, O_CREAT|O_EXCL, FILE_MODE)) < 0) {
                    if (errno == EEXIST)
                        continue;
                    else
                        err_sys("open LKF2 error");
                } else {
                    break;
                }
            }
            counter = update((long *)area);
            printf("child: i = %d; area = %ld; counter = %d\n", 
                    i, *((long *)area), counter);
            unlink(LKF1);
        }
    }
        
    exit(0);
}
