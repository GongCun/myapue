#include <limits.h>
#include <sys/stat.h>
#include "myapue.h"

#define RWXRWXRWX (S_IRWXU|S_IRWXG|S_IRWXO)
#define NAME "alonglonglonglonglonglongname"
#define MAXSZ 8192

int main(void)
{
    umask(022);
    int i;
    size_t size;
    char *pathbuf;

    for (i = 0; i < 100; i++) {
        if (mkdir(NAME, RWXRWXRWX) < 0)
            err_sys("mkdir error");
        if (chdir(NAME) < 0)
            err_sys("chdir error");
    }

    pathbuf = path_alloc(&size);
    for(;;) {
        if (getcwd(pathbuf, size) != NULL) {
            break;
        } else {
            err_ret("getcwd failed, size = %d", size);
            size += 100;
            if (size > MAXSZ) 
                err_quit("gaving up");
            if ((pathbuf = realloc(pathbuf, size)) == NULL)
                err_sys("realloc error");
        }
    }
    printf("length = %ld\n%s\n", (long)strlen(pathbuf), pathbuf);
    exit(0);
}

