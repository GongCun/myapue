#include "open.h"

#define MAXARGC 50 /* max number of arguments in buf */
#define WHITE " \t\n" /* white space for tokenizing arguments */

/*
 * buf[] contains white-space-separated arguments. we convert it to an
 * argv-style array of pointers, and call the user's function (optfunc)
 * to process the array. we return -1 if there's a problem parsing buf,
 * else we return whatever optfunc() returns. note that user's buf[]
 * array is modified (nulls placed after each token)
 */
int buf_args(char *buf, int (*optfunc)(int, char **))
{
    char *ptr, **argv;
    int argc;

    argv = malloc(sizeof(char *));
    if (argv == NULL)
        err_sys("malloc error");
    if (strtok(buf, WHITE) == NULL) /* an argv[0] is required */
        return(-1);
    argv[argc = 0] = malloc(sizeof(char));
    argv[argc] = buf;
    while ((ptr = strtok(NULL, WHITE)) != NULL) {
        if(++argc >= MAXARGC-1) /* -1 for room for NULL at end */
            return(-1);
        argv[argc] = malloc(sizeof(char)); 
        argv[argc] = ptr;
        /* argv[argc] = ptr; */
    }
    argv[++argc] = NULL;

    /*
     * since argv[] pointers point into the user's buf[],
     * user's function can just copy the pointers, even
     * though argv[] array will disappear on return.
     */
    return((*optfunc)(argc, argv));
}
