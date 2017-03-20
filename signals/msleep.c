#include <setjmp.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include "myapue.h"

extern unsigned int sleep1(unsigned int);
extern unsigned int sleep2(unsigned int);

int main(void)
{
    unsigned int unslept;

    /* unslept = sleep1(5); */
    unslept = sleep2(5);
    printf("sleep1 returned: %u\n", unslept);

    return 0;
}
