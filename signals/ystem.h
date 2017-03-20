#ifndef _YSTEM_H
#define _YSTEM_H

#include <sys/wait.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>

int ystem(const char *cmdstring);

#endif
