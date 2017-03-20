#include "myapue.h"

int main(void)
{
    char *line;

    line = "temp";
    if ( mkstemp(line) < 0 )
        err_sys("mkstemp error");
    unlink(line);
    printf("%s\n", line);
    exit(0);
}
