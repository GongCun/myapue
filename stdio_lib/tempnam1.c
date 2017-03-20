#include "myapue.h"

int main(int argc, char **argv)
{
    if (argc != 3)
        err_quit("usage: tempnam <directory> <prefix>");
    printf("%s\n", tempnam(argv[1][0] != ' ' ? argv[1] : NULL,
                argv[2][0] != ' ' ? argv[2] : NULL));
    exit(0);
}
