ROOT=..
include $(ROOT)/Make.defines

PROGS = getpwd1 getpwd2 getmaxgrp getgroup1 uname1 strftime1 \
		strftime2 ctime1 ctime2 ctime3 getmaxtime1

all:	clean1 $(PROGS)

$(PROGS): %: %.o
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS) $(LDLIBS) 

clean:
	rm -vf *.o *~ $(PROGS)

.PHONY: clean1

clean1:
	rm -vf *~

include $(ROOT)/Make.libmyapue.inc
