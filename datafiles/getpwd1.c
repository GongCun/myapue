#include "myapue.h"
#include <sys/types.h>
#include <pwd.h>
#include <uuid/uuid.h>
#include <unistd.h>

static void getname(const char *str, uid_t uid)
{
    struct passwd *pwdptr;

    if ( ( pwdptr = getpwuid(uid) ) == NULL )
        err_sys("getpwuid error");
    printf("%s %s\n", str, pwdptr->pw_name);
    return;
}

int main(void)
{
    uid_t uid;

    uid = getuid();
    getname("user name: ", uid);
    uid = geteuid();
    getname("effective user name: ", uid);
    
    exit(0);
}


