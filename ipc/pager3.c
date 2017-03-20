#include "myapue.h"
#include <sys/wait.h>

#define PAGER "${PAGER:-more}"

int main(int argc, char **argv)
{
    char line[MAXLINE];
    FILE *fpin, *fpout;

    if (argc != 2)
        err_quit("%s <pathname>", *argv);
    if ((fpin = fopen(argv[1], "r")) == NULL)
        err_sys("can't open %s", argv[1]);
    if ((fpout = mpopen(PAGER, "w")) == NULL)
        err_sys("mpopen error");

    /* copy argv[1] to pager */
    while (fgets(line, MAXLINE, fpin) != NULL) {
        if (fputs(line, fpout) == EOF)
            err_sys("fputs error to pipe");
    }
    if (ferror(fpin))
        err_sys("fgets error");
    if (mpclose(fpout) == -1)
        err_sys("mpclose error");
    exit(0);
}
