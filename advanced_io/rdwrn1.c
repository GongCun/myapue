#include "myapue.h"

int main(void)
{
    int n;
    char buf[MAXLINE];

    while ( (n = readn(STDIN_FILENO, buf, MAXLINE)) > 0 ) {
        if (writen(STDOUT_FILENO, buf, n) != n)
            err_sys("writen error");
    }
    exit(0);
}
