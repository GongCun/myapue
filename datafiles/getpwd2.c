#include <pwd.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct passwd *
getpwnam1(const char *name)
{
    struct passwd *ptr;

    setpwent();
    while ( ( ptr = getpwent() ) != NULL)
        if ( strcmp(name, ptr->pw_name) == 0 )
            break;
    endpwent();
    return(ptr);
}

static void gettime(time_t *clock)
{
    struct tm *tmptr;

    if ( (tmptr = localtime(clock)) != NULL ) {
        printf("%d %d %d %d:%d:%d\n",
                tmptr->tm_year + 1900,
                tmptr->tm_mon + 1,
                tmptr->tm_mday,
                tmptr->tm_hour,
                tmptr->tm_min,
                tmptr->tm_sec);
    }
}

int main(void)
{
    struct passwd *ptr;

    if ( ( ptr = getpwnam1("gongcunjust") ) != NULL ) {
        printf("pw_name: %s\n", ptr->pw_name);
        printf("pw_passwd: %s\n", ptr->pw_passwd);
        printf("pw_uid: %lu\n", (unsigned long)ptr->pw_uid);
        printf("pw_gid: %lu\n", (unsigned long)ptr->pw_gid);
        printf("pw_gecos: %s\n", ptr->pw_gecos);
        printf("pw_dir: %s\n", ptr->pw_dir);
        printf("pw_shell: %s\n", ptr->pw_shell);
        printf("pw_class: %s\n", ptr->pw_class);
        printf("pw_change: ");
        gettime(&ptr->pw_change);
        printf("pw_expire: ");
        gettime(&ptr->pw_expire);
    }
    exit(0);
}
