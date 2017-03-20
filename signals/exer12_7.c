#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define BUFFSIZE 60000000

int main(void)
{
    FILE *in;
    unsigned long a[1024][512];
    char buf[BUFFSIZE];
    int i;

    in = fopen("./exer12.data", "w");
    if (!in) {
        perror("fopen");
        return -1;
    }

    if (setvbuf(in, buf, _IOFBF, BUFFSIZE) != 0)
    {
        perror("setvbuf");
        return -1;
    }

    for (i = 0; i < 125; i++) 
        if (!fwrite(&a, sizeof(a), 1, in)) {
            perror("fwrite");
            return -1;
        }

    if (fclose(in)) {
        perror("fclose");
        return -1;
    }

    return 0;
}
