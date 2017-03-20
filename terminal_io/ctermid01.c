#include "myapue.h"

int main(void)
{
    char ptr[L_ctermid];
    if (ctermid(ptr) == NULL)
        err_sys("ctermid error");
    printf("termid = %s\n", ptr);
    exit(0);
}
