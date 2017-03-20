#include "myapue.h"
#include <time.h>

int main(void)
{
    time_t start, end, mid;
    time_t i;

    start = 36028797002186752;
    end = 72057594004373504;
    while (start < end) {
        mid = (start + end) / 2;
        if (mid == start || mid == end)
            break;
        if (ctime(&mid) == NULL)
            end = mid;
        else
            start = mid;
    }

    printf("%ld\n", (long)start);

    i = start - 1;
    printf("%s\n", ctime(&i));
    printf("%s\n", ctime(&start));
    i = start + 1;
    printf("%s\n", ctime(&i));

    exit(0);
}
