#include "syscalls.h"

KRFILE _iob[OPEN_MAX] = { /* stdin, stdout, stderr */
    { 0, (char *)0, (char *)0, _READ, STDIN_FILENO },
    { 0, (char *)0, (char *)0, _WRITE, STDOUT_FILENO },
    { 0, (char *)0, (char *)0, _WRITE|_UNBUF, STDERR_FILENO }
};

int main(void)
{
    int c;
    KRFILE *fp;
    char *ptr;

    fp = krfopen("/dev/tty", "r");
    if (fp == NULL)
        err_sys("krfopen error");

    if ((c = krgetc(fp)) == EOF)
        err_sys("krgetc error");

    printf("character = %c\n", c);

    c = getc(stdin);
    printf("stdio character = %c\n", c);

    if (krputc(c, krstdout) == EOF)
        err_sys("krputc error");
    xfflush(krstdout);

    printf("\n");

    fp = krfopen("temp.file", "w");
    if (fp == NULL) 
        err_sys("krfopen error");
    if (krfseek(fp, 1024, SEEK_SET) < 0) 
        err_sys("krfseek error");
    if (krputc(c, fp) == EOF)
        err_sys("krputc error");
    xfflush(fp);

    if (krfclose(fp) == EOF)
        err_sys("krfclose error");

    /*
     * test malloc
     */
    ptr = (char *) krmalloc(32);
    snprintf(ptr, 32, "krmalloc string");
    printf("%s\n", ptr);
    krfree(ptr);

    /*
     * test calloc
     */
    ptr = (char *) krcalloc(1, 32);
    snprintf(ptr, 32, "krcalloc string");
    printf("%s\n", ptr);
    krfree(ptr);

    /*
     * test safe malloc
     */
    ptr = (char *) krmalloc_safe(10241); /* will error */
    ptr = (char *) krmalloc_safe(32); 
    snprintf(ptr, 32, "krmalloc_safe string");
    krfree_safe(ptr);

    /*
     * test bfree
     */
    char *p = (char *) krmalloc(32);
    printf("bfree %u\n", bfree(p, 32));


    exit(0);
}

