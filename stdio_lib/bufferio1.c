#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    FILE *in, *out;
    struct pirate {
        char name[100];
        unsigned long booty;
        unsigned int beard_len;
    }p, blackbeard = {"Edward Teach", 950, 48};
    
    out = fopen("data", "w");
    if (!out) {
        perror("fopen");
        exit(1);
    }

    if (fwrite(&blackbeard, sizeof(struct pirate), 1, out) != 1) {
        perror("fwrite");
        exit(1);
    }
    if (fclose(out) != 0) {
        perror("fclose");
        exit(1);
    }

    in = fopen("data", "r");
    if (!in) {
        perror("fopen");
        exit(1);
    }

    if (fread(&p, sizeof(struct pirate), 1, in) != 1) {
        if (ferror(in)) {
            perror("fread");
            exit(1);
        }
    }

    if (fclose(in) != 0) {
        perror("fclose");
        exit(1);
    }

    printf("name=\"%s\", booty=%lu, bread_len=%u\n",
            p.name, p.booty, p.beard_len);
    exit(0);
}

