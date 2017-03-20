#include "myapue.h"
#include <pwd.h>
#include <unistd.h>

int main(void)
{
    char *ptr;

    ptr = getpass("Enter password:");
    if (ptr == NULL)
        err_sys("getpass error");
    printf("password: %s\n", ptr);
    exit(0);
}
