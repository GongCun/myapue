#include "myapue.h"
#include <sys/socket.h>

int main(void)
{
    int n, fd;
    char buf[MAXLINE];
    FILE *fp;

    if ((n = read(STDIN_FILENO, buf, sizeof(buf))) < 0)
        err_sys("read error");
    buf[n] = 0;
    /* buf[strlen(buf)] = 0; */

    fp = fopen("./temp.file", "w+");
    fprintf(fp, "pathnam = %s\n", buf);
    fprintf(fp, "n = %d\n", n);
    fprintf(fp, "strlen = %zd\n", strlen(buf));
    fclose(fp);

    fd = open(buf, O_RDONLY);
    if (fd < 0)
        err_sys("open error");
    send_fd(STDOUT_FILENO, fd);

    exit(0);
}
    
