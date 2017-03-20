#include <ftw.h>
#include <stdio.h>
#include <stdlib.h>

int fn(const char *name, const struct stat *statbuf, int flag)
{
    char *ptr;

    printf("%-20s: ", name);
    switch(flag) {
        case FTW_F: 
            ptr = "regular";
            break;
        case FTW_D:
            ptr = "directory";
            break;
        case FTW_DNR:
            ptr = "directory cannot be read";
            break;
        case FTW_NS:
            ptr = "no stat information was available";
            break;
        case FTW_SL:
            ptr = "symbolic link";
            break;
        default:
            ptr = "*** unknown mode ***";
    }

    printf("%-30s\n", ptr); 
    return 0;
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        printf("ftw1 <directory_name>\n");
        exit(1);
    }
    ftw(argv[1], fn, 1);
    exit(0);
}



