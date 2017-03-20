#include "myapue.h"

static char ctermid_name[L_ctermid];

static char *my_ctermid(char *ptr)
{
    if (ptr == NULL)
        ptr = ctermid_name;
    return(strcpy(ptr, "/dev/tty"));
}

int main(void)
{
    printf("termid = %s\n", my_ctermid(NULL));
    exit(0);
}
