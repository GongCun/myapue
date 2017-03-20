#include "myapue.h"
#include <sys/wait.h>

int main(void)
{
    pid_t pid;
    FILE *fp;
    int rc;


    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) { 
        if ((fp = popen("/usr/bin/true", "r")) == NULL) 
            err_sys("popen error"); 
        if ((rc = system("sleep 10")) < 0) 
            err_sys("system() error"); 
        if (pclose(fp) < 0) 
            err_sys("pclose error");
        exit(0);
    }

    /* parent process continue... */
    if (wait(NULL) < 0)
        err_sys("wait error");
    exit(0);
}
    
