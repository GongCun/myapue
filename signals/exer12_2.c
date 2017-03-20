#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(void)
{
    FILE *in;
    struct {
        unsigned long a[1024][1024];
    } item;
    char buf[BUFSIZ];

    in = fopen("./exer12.data", "w");
    if (!in) {
        perror("fopen");
        return -1;
    }

    setbuf(in, buf);

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
