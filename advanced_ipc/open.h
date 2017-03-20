#ifndef _OPEN_H
#define _OPEN_H

#include "myapue.h"
#include <errno.h>

#define CL_OPEN "open" /* client's request for server */

extern char errmsg[]; /* error message string to return client */
extern int oflag; /* open() flag: O_xxx ... */
extern char *pathname; /* of file to open() for client */

int csopen(char *, int);
int cli_args(int, char **);
int buf_args(char *, int (*optfunc)(int, char **));
void request(char *, int, int);

#endif
