#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pwd.h>

int main(void)
{
    struct passwd *pw;
    pw = getpwnam("gongcunjust");
    printf("name = %s\n", pw->pw_name);
    printf("password = %s\n", pw->pw_passwd);
    printf("user id = %d\n", (int)pw->pw_uid);
    printf("group id = %d\n", (int)pw->pw_gid);
    printf("real name = %s\n", pw->pw_gecos);
    printf("home dir = %s\n", pw->pw_dir);
    printf("shell = %s\n", pw->pw_shell);
    return 0;
}
    
