#include "myapue.h"
#include <ctype.h>

int main(void)
{
    char c;
    while ((c = getchar()) != EOF) {
        if (isalpha(c))
            c = tolower(c);
        if (putchar(c) == EOF)
            err_sys("putchar error");
        if (c == '\n')
            fflush(stdout);
    }
    exit(0);
}

