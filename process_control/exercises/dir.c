#include <dirent.h>
#include <errno.h>
#include "myapue.h"

DIR *dir;

void open_dir(const char *s)
{
    if ((dir = opendir(s)) == NULL)
        err_sys("opendir error");
}
