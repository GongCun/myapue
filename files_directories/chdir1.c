#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "myapue.h"

int main(void)
{
    if (chdir("/tmp") < 0)
        err_sys("chdir failed");
    printf("chdir to /tmp successed\n");
    system("ls -l");
    exit(0);
}
