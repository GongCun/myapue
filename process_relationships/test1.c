#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include "myapue.h"

extern void pr_ids(char *);

int main(void)
{
    pr_ids("test1");

    return 0;
}
