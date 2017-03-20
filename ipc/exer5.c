#include "myapue.h"

static void sig_pipe(int);

int main(void)
{
    int fd1[2], fd2[2];
    pid_t pid;
    char line[MAXLINE];
    FILE *fp1, *fp2;

    if (signal(SIGPIPE, sig_pipe) == SIG_ERR)
        err_sys("signal error");
    if (pipe(fd1) < 0 || pipe(fd2) < 0)
        err_sys("pipe error");

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {
        close(fd1[1]);
        close(fd2[0]);
        if (fd1[0] != STDIN_FILENO) {
            if (dup2(fd1[0], STDIN_FILENO) != STDIN_FILENO)
                err_sys("dup2 error to stdin");
            close(fd1[0]);
        }
        if (fd2[1] != STDOUT_FILENO) {
            if (dup2(fd2[1], STDOUT_FILENO) != STDOUT_FILENO)
                err_sys("dup2 error to stdout");
            close(fd2[1]);
        }
        if (execl("add", "add", (char *)0) < 0)
            err_sys("execl error");
        _exit(127);
    }

    /* parent continue... */
    close(fd1[0]);
    close(fd2[1]);
    if ((fp1 = fdopen(fd1[1], "w")) == NULL)
        err_sys("fdopen for write error");
    if ((fp2 = fdopen(fd2[0], "r")) == NULL)
        err_sys("fdopen for read error");
    if (setvbuf(fp1, NULL, _IOLBF, 0) == EOF ||
            setvbuf(fp2, NULL, _IOLBF, 0) == EOF)
        err_sys("setvbuf error");
    
    while (fgets(line, MAXLINE, stdin) != NULL) {
        if (fputs(line, fp1) == EOF) 
            err_sys("fputs error to pipe");
        if (fgets(line, sizeof(line), fp2) == NULL) {
            if (ferror(fp2)) 
                err_sys("fgets error from pipe");
            else { 
                err_msg("child closed pipe"); 
                break;
            }
        }
        line[strlen(line)-1] = 0;
        if (fputs(line, stdout) == EOF)
            err_sys("fputs error");
        fputc('\n', stdout);
    }
    if (ferror(stdin))
        err_sys("fgets error on stdin");
    if (waitpid(pid, NULL, 0) < 0)
        err_sys("waitpid error");
    exit(0);
}

static void sig_pipe(int signo)
{
    printf("SIGPIPE caught\n");
    exit(1);
}
