#include <sys/types.h>
#include <pwd.h>
#include <uuid/uuid.h>
#include <unistd.h>
#include <stdio.h>

int main(void)
{
    char *buf;
    struct passwd *pwp;

    if ((pwp = getpwuid(getuid())) != NULL) {
        printf("%s\n", pwp -> pw_name);
    }

    if ((buf = getlogin()) != NULL) {
        printf("%s\n", buf);
    }

    if ((pwp = getpwuid(0)) != NULL) {
        printf("%s\n", pwp -> pw_name);
    } else {
        printf("no login name\n");
    }

    return 0;
}
