#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main(int argc, char **argv)
{
    char *ptr;
    long pathmax;

    if (argc != 2) {
        printf("getcwd1 <pathname>\n");
        exit(1);
    }

    if (chdir(argv[1]) < 0) {
        perror("chdir");
        exit(1);
    }

    errno = 0;
    if ((pathmax = pathconf("/", _PC_PATH_MAX)) < 0) {
        if (errno == 0)
            pathmax = 1024;
        else {
            perror("pathconf");
            exit(1);
        }
    } else {
        pathmax++; /* add the terminate null */
    }

    if (pathmax <= strlen(argv[1])) {
        pathmax = strlen(argv[1]) * 2;
    }
    if ((ptr = malloc(pathmax)) == NULL) {
        perror("malloc");
        exit(1);
    }

    if (getcwd(ptr, pathmax) == NULL) {
        perror("getcwd");
        exit(1);
    }
    printf("cwd = %s\n", ptr);
    exit(0);
}
