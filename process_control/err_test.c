#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "myapue.h"

int main(void)
{
    int a = 10;
    char string[] = "err test";

    err_sys("it is a test: %d : %s", a, string);
    return 0;
}
