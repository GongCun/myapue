#ifndef _OPEN_H
#define _OPEN_H

#include "myapue.h"
#include <errno.h>

#define CL_OPEN "open" /* client's request for server */

/* server's well-known name, absolute path for daemon */
#define CS_OPEN "/Users/gongcunjust/c_program/sys_program/advanced_ipc/open2/opend" 

extern int debug; /* nonzero if interactive (not daemon) */
extern char errmsg[]; /* error message string to return client */
extern int oflag; /* open() flag: O_xxx ... */
extern char *pathname; /* of file to open() for client */

typedef struct { /* one Client struct per connected client */
    int fd; /* fd, or -1 if available */
    uid_t uid;
} Client;

extern Client *client; /* ptr to malloc'ed array */
extern int client_size; /* # entries in client[] array */

int csopen(char *, int);
int cli_args(int, char **);
int client_add(int, uid_t);
void client_del(int);
void loop_select(void);
void loop_poll(void);
int buf_args(char *, int (*optfunc)(int, char **));
void request(char *, int, int, uid_t);

#endif
