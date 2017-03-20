#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char **argv)
{
    DIR *dp;
    struct dirent *dirp;

    if (argc != 2) {
        printf("usage ls1 <directory_name>\n");
        exit(1);
    }

    if ((dp = opendir(argv[1])) == NULL) {
        perror("opendir");
        exit(1);
    }

    while ((dirp = readdir(dp)) != NULL)
        printf("%s\n", dirp->d_name);

    closedir(dp);
    exit(0);
}


