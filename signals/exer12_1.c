#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

/* maximun integer size on 64bit is 2147483647 */
#define BUFFSIZE 1

int main(void)
{
    FILE *in;
    struct {
        unsigned long a[60000][10];
    } item;
    char *buf;

    if ((buf = (char *)malloc(BUFFSIZE)) == NULL)
    {
        perror("malloc");
        return -1;
    }

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

    if (!fwrite(&item, sizeof(item), 1, in)) {
        perror("fwrite");
        return -1;
    }

    if (fclose(in)) {
        perror("fclose");
        return -1;
    }

    return 0;
}
