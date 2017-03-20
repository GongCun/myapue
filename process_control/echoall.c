#include <stdio.h>
#include <stdlib.h>
#include "myapue.h"

int main(int argc, char **argv)
{
    int i;
    char **ptr;
    extern char **environ;

    for (i = 0; i < argc; i++)
        printf("argv[%d] = %s\n", i, argv[i]);
    
    for (ptr = environ; *ptr != 0; ptr++) /* and all env string */
        printf("%s\n", *ptr);

    return 0;
}
