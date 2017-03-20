#include "myapue.h"

int main(void)
{
    char *ptr;
    if ((ptr = ctermid(NULL)) == NULL)
        err_sys("ctermid error");
    printf("termid = %s\n", ptr);
    exit(0);
}
