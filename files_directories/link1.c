#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(void)
{
    if (link("foo", "foo.link") < 0) {
        perror("link");
        exit(1);
    }
    exit(0);
}
