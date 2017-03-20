#include "myapue.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <time.h>

#define SERV "aioserv"

int listenfd, connfd;

static void pr_stat(int, char *);

int main(int argc, char **argv)
{
    int n, on = 1;
    char *host;
    char buff[100];
    int fd;

    n = (int)sysconf(_SC_HOST_NAME_MAX);
    if (n < 0)
        err_sys("sysconf error");
    if ((host = malloc(n)) == NULL)
        err_sys("malloc error");
    if (gethostname(host, n) < 0)
        err_sys("gethostname error");

    listenfd = Tcp_listen(host, SERV, NULL);
    fd = open("./temp.file", O_RDWR|O_CREAT, FILE_MODE);
    if (fd < 0)
        err_sys("open error");
    pr_stat(listenfd, "socket");
    pr_stat(fd, "regular file");
    pr_stat(STDIN_FILENO, "character device");
}

static void pr_stat(int fd, char *str)
{
    struct stat statbuf;
    char *buf = calloc(4096, 1);
    if (buf == NULL)
        err_sys("malloc error");
    char *ptr = buf;

    printf("%s:\n", str);

    if (fstat(fd, &statbuf) < 0)
        err_sys("fstat error");

    if (S_ISSOCK(statbuf.st_mode))
        strcpy(buf, "st_mode: socket\n");
    else
        strcpy(buf, "st_mode: other\n");

    strcat(buf, "st_mode: ");
    if (statbuf.st_mode & S_IRUSR) 
        strcat(buf, "user-read ");
    if (statbuf.st_mode & S_IWUSR) 
        strcat(buf, "user-write ");
    if (statbuf.st_mode & S_IXUSR) 
        strcat(buf, "user-execute ");
    if (statbuf.st_mode & S_IRGRP) 
        strcat(buf, "group-read ");
    if (statbuf.st_mode & S_IWGRP) 
        strcat(buf, "group-write ");
    if (statbuf.st_mode & S_IXGRP) 
        strcat(buf, "group-execute ");
    if (statbuf.st_mode & S_IROTH) 
        strcat(buf, "other-read ");
    if (statbuf.st_mode & S_IWOTH) 
        strcat(buf, "other-write ");
    if (statbuf.st_mode & S_IXOTH) 
        strcat(buf, "other-execute ");
    strcat(buf, "\n");
    ptr = buf + strlen(buf);
    sprintf(ptr, "st_ino: i-node number = %lld\n", statbuf.st_ino);
    ptr = buf + strlen(buf);
    sprintf(ptr, "st_dev: dev = %d/%d\n", 
            major(statbuf.st_dev), minor(statbuf.st_dev));
    ptr = buf + strlen(buf);
    sprintf(ptr, "st_rdev: rdev = %d/%d\n", 
            major(statbuf.st_rdev), minor(statbuf.st_rdev));
    ptr = buf + strlen(buf);
    sprintf(ptr, "st_nlink: number of links = %d\n", statbuf.st_nlink);
    ptr = buf + strlen(buf);
    sprintf(ptr, "st_uid: user id of owner = %d\n", statbuf.st_uid);
    ptr = buf + strlen(buf);
    sprintf(ptr, "st_gid: group id of owner = %d\n", statbuf.st_gid);
    ptr = buf + strlen(buf);
    sprintf(ptr, "st_size: size in bytes = %lld\n", statbuf.st_size);
    ptr = buf + strlen(buf);
    sprintf(ptr, "st_blksize: best I/O block size = %d\n", statbuf.st_blksize);
    ptr = buf + strlen(buf);
    sprintf(ptr, "st_blocks: number of disk blocks = %lld\n", 
            statbuf.st_blocks);
    ptr = buf + strlen(buf);
    sprintf(ptr, "st_atimespec: time of last access: %s", 
            ctime(&statbuf.st_atimespec.tv_sec));
    ptr = buf + strlen(buf);
    sprintf(ptr, "st_mtimespec: time of last modification: %s", 
            ctime(&statbuf.st_mtimespec.tv_sec));
    ptr = buf + strlen(buf);
    sprintf(ptr, "st_ctimespec: time of last file status change: %s", 
            ctime(&statbuf.st_ctimespec.tv_sec));
    printf("%s", buf);
    return;
}


