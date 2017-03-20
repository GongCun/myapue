#include <stdio.h>
#include <stdlib.h>

void setbuf1(FILE *fp, char *buf)
{
    setvbuf(fp, buf, buf ? _IOFBF : _IONBF, BUFSIZ);
    return;
}

int main(void)
{
    char buf[BUFSIZ];

    setbuf1(stdout, buf);
    printf("stdout: ");
    if (stdout->_flags & __SNBF)
        printf("unbuffered");
    else if (stdout->_flags & __SLBF)
        printf("line buffered");
    else
        printf("full buffered");
    putc('\n', stdout);

    setbuf1(stdin, NULL);
    printf("stdin: ");
    if (stdin->_flags & __SNBF)
        printf("unbuffered");
    else if (stdin->_flags & __SLBF)
        printf("line buffered");
    else
        printf("full buffered");
    putc('\n', stdout);
    
    exit(0);
}
