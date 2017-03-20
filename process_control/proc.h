#ifndef _PROC_H_
#define _PROC_H_

#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>

int system(const char *cmdstring);

#endif
