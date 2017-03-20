#include <stdlib.h>
#include <stdio.h>

int main(void)
{
    printf("%s\n", getenv("COLUMNS"));
    printf("%s\n", getenv("HOME"));
    printf("%s\n", getenv("SHELL"));
    return 0;
}
