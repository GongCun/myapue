#include <sys/resource.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#define doit(name) pr_limits(#name, name)

static void pr_limits(char *, int);

int main(void) 
{
#ifdef RLIMIT_AS
    doit(RLIMIT_AS); /* max memory size */
#endif

    doit(RLIMIT_CORE); /* max size in bytes of a core file */
    doit(RLIMIT_CPU); /* max amount of cpu time in seconds */
    doit(RLIMIT_DATA); /* max size in bytes of the data segment : */
                       /* the sum of the initialized data, uninitialized data, and heep */
    doit(RLIMIT_FSIZE); /* the max size in bytes of a file that may be created */

#ifdef RLIMIT_MEMLOCK
    doit(RLIMIT_MEMLOCK); /* max amount mem in bytes that a process can lock into memory using mlock */
#endif

#ifdef RLIMIT_MSGQUEUE
    doit(RLIMIT_MSGQUEUE); /* max mem a process can allocate for POSIX message queue */
#endif

#ifdef RLIMIT_NICE
    doit(RLIMIT_NICE); /* the limit to which a process's nice value can be raised */
#endif

    doit(RLIMIT_NOFILE);

#ifdef RLIMIT_NPROC
    doit(RLIMIT_NPROC); /* the max number of child processes per real user ID */
#endif

#ifdef RLIMIT_NPTS
    doit(RLIMIT_NPTS); /* the max number of pseudo terminals that a user can open at one time */
#endif

#ifdef RLIMIT_RSS
    doit(RLIMIT_RSS); /* max resident set size in bytes */
#endif

#ifdef RLIMIT_SBSIZE
    doit(RLIMIT_SBSIZE); /* max size of socket buffers that a user can consume at any given time */
#endif

#ifdef RLIMIT_SIGPENDING
    doit(RLIMIT_SIGPENDING); /* max number of signal that can be queued for a process */
#endif

    doit(RLIMIT_STACK); /* max size in bytes of the stack */

#ifdef RLIMIT_SWAP
    doit(RLIMIT_SWAP); /* tha max amount of swap space in bytes that a user can consume */
#endif

#ifdef RLIMIT_VMEM
    doit(RLIMIT_MEM); /* a synonym for RLIMIT_AS */
#endif

    return 0;
}

static void pr_limits(char *name, int resource)
{
    struct rlimit limit;
    unsigned long long lim;

    if (getrlimit(resource, &limit) < 0) {
        perror("errno");
        exit(1);
    }

    printf("%-14s ", name);
    if (limit.rlim_cur == RLIM_INFINITY) {
        printf("(infinite) ");
    } else {
        lim = limit.rlim_cur;
        printf("%10lld ", lim);
    }
    if (limit.rlim_max == RLIM_INFINITY) {
        printf("(infinite) ");
    } else {
        lim = limit.rlim_max;
        printf("%10lld ", lim);
    }
    putchar((int)'\n');
}
    

