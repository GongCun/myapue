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
    pid_t pid;
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


    /*
     * the fork() child process will inherit the saved user ID
     * of its parent
     */
    if ((pid = fork()) < 0) {
        perror("fork");
        exit(1);
    } else if (pid == 0) {
        access_test(argv[1]);
    }

    exit(0);
}
