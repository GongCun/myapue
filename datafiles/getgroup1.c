#include <grp.h>
#include <pwd.h>
#include "myapue.h"

int main(void)
{
    struct group *grptr;
    gid_t *grouplist;
    int n, i;

    if ( (n = getgroups(0, NULL)) < 0 )
        err_sys("getgroups error");
    grouplist = malloc(sizeof(gid_t) * n);
    
    if ( getgroups(n, grouplist) < 0 )
        err_sys("getgroups error");
    for (i = 0; i < n; i++) {
        if ( (grptr = getgrgid(grouplist[i])) == NULL )
            err_sys("getgrgid error");
        printf("%d %s\n", grptr->gr_gid, grptr->gr_name);
    }
    return 0;
}
