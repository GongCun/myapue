#include <dirent.h>
#include <limits.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "myapue.h"

/* function type that is called for each filename */
typedef int Myfunc(const char *, const struct stat *, int);
static Myfunc myfunc;
static int myftw(char *, Myfunc *);
static int dopath(Myfunc *);

int main(int argc, char **argv)
{
    int ret;

    if (argc != 2)
        err_quit("usage: ftw2 <starting-pathname>");

    ret = myftw(argv[1], myfunc); /* does it all */

    exit(ret);
}

#define FTW_F 1
#define FTW_D 2
#define FTW_DNR 3
#define FTW_NS 4

static char *fullpath;
static size_t pathlen;

static int myftw(char *pathname, Myfunc *func)
{
    fullpath = path_alloc(&pathlen); /* malloc PATH_MAX+1 bytes */
    if (pathlen <= strlen(pathname)) {
        pathlen = strlen(pathname) * 2;
        if ((fullpath = realloc(fullpath, pathlen)) == NULL)
            err_sys("realloc failed");
    }
    strcpy(fullpath, pathname);
    return(dopath(func));
}

/*
 * desend through the hierarchy, starting at "fullpath".
 * if "fullpath" is anything other than a directory, we lstat() it,
 * call func(), and return. For a directory, we call ourself
 * recursively for each name in the directory.
 */
static int dopath(Myfunc *func)
{
    struct stat statbuf;
    struct dirent *dirp;
    DIR *dp;
    int ret, n;

    if (lstat(fullpath, &statbuf) < 0) /* stat error */
        return(func(fullpath, &statbuf, FTW_NS));
    if (!S_ISDIR(statbuf.st_mode)) /* not a directory */
        return(func(fullpath, &statbuf, FTW_F)); /* as regular file */

    /*
     * it's a directory. first call func() for the directory,
     * then process each filename in the firectory.
     */
    if ((ret = func(fullpath, &statbuf, FTW_D)) != 0)
        return(ret);

    n = strlen(fullpath);
    if (n + NAME_MAX + 2 > pathlen) { /* expand path buffer */
       pathlen *= 2; 
       if ((fullpath = realloc(fullpath, pathlen)) == NULL)
           err_sys("realloc failed");
    }
    fullpath[n++] = '/';
    fullpath[n] = 0;

    if ((dp = opendir(fullpath)) == NULL) /* cannot read directory */
        return func(fullpath, &statbuf, FTW_DNR);

    while ((dirp = readdir(dp)) != NULL) {
        if (strcmp(dirp->d_name, ".") == 0 ||
                strcmp(dirp->d_name, "..") == 0)
            continue;
        strcpy(fullpath+n, dirp->d_name); /* append name after slash */
        if ((ret = dopath(func)) != 0) /* recursive */
            break; /* time to leave */
    }
    fullpath[n-1] = 0; /* erase everything from slash onward */
    if (closedir(dp) < 0)
        err_ret("cannot close directory %s", fullpath);
    return(ret);
}

static int myfunc(const char *pathname, const struct stat *statptr, int type)
{
    char *ptr;
    printf("%s : ", pathname);
    switch(type) {
        case(FTW_F):
            switch(statptr->st_mode & S_IFMT) {
                case S_IFREG: ptr = "regular"; break;
                case S_IFBLK: ptr = "block"; break;
                case S_IFCHR: ptr = "character"; break;
                case S_IFLNK: ptr = "link"; break;
                case S_IFIFO: ptr = "fifo"; break;
                case S_IFSOCK: ptr = "socket"; break;
                case S_IFDIR: err_dump("for S_IFDIR for %s", pathname);
            }
            break;
        case(FTW_D): 
            ptr = "directory"; 
            break; 
        case(FTW_DNR): 
            err_ret("cannot read directory %s", pathname);
            break;
        case(FTW_NS):
            err_ret("stat error for %s", pathname);
            break;
        default:
            err_dump("unknown type %d for pathname: %s", type, pathname);
    }
    printf("%s\n", ptr);
    return 0;
}

