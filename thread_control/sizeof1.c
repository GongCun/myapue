#include <stdio.h>
#include <string.h>

int main(void)
{
    char buf[10] = "test";

    printf("sizeof = %ld; strlen = %ld\n", sizeof(buf), strlen(buf));
    printf("sizeof = %ld; strlen = %ld\n", sizeof(&buf[1]), strlen(&buf[1]));
    printf("sizeof = %ld; strlen = %ld\n", sizeof(buf+2), strlen(&buf[2]));
    printf("sizeof = %ld; strlen = %ld\n", sizeof(buf+3), strlen(&buf[3]));

    return 0;
}

