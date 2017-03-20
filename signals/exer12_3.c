#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(void)
{
    FILE *in;
    unsigned long a[1024][1024];
    char buf[BUFSIZ];

    printf("sizeof(a) = %ld\n", (long)sizeof(a));

    in = fopen("./exer12.data", "w");
    if (!in) {
        perror("fopen");
        return -1;
    }

    setbuf(in, buf);

    if (!fwrite(a, sizeof(a), 1, in)) {
        perror("fwrite");
        return -1;
    }

    if (fclose(in)) {
        perror("fclose");
        return -1;
    }

    return 0;
}
