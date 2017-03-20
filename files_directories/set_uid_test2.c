#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>

void access_test(char *str)
{
    if (access(str, R_OK) < 0) {
        perror("access for file");
    } else {
        printf("read access OK\n");
    }

    if (open(str, O_RDONLY) < 0) {
        perror("open for file");
    } else {
        printf("open for reading OK\n");
    }
    return;
}


int main(int argc, char **argv)
{
    /*
     * the program's effective uid is root
     * set its effective uid = real uid
     * then test its access permission
     */
    if (argc != 2)
        exit(1);

    access_test(argv[1]);
    setuid(getuid());
    access_test(argv[1]);
    seteuid(geteuid());
    access_test(argv[1]);

    exit(0);
}
