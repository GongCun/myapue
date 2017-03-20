#ifndef _SIGNAL1_H
#define _SIGNAL1_H

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "myapue.h"

typedef void Sigfunc(int);

/* reliable version of signal(), using POSIX sigaction */
Sigfunc *signal1(int, Sigfunc *);

/* prevent any interrupted system call from being restarted */
Sigfunc *signal_inr(int , Sigfunc *);

/* POSIX-style abort() function */
void abort1(void);

#endif

