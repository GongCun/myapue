ROOT=..
include $(ROOT)/Make.defines

PROGS = detach1

all:	$(PROGS) clean1 join1 recursive1 recursive2 getenv1 getenv2 nonreentrant_getenv1 nonreentrant_getenv2 \
	nonreentrant_getenv3 reentrant_getenv1 reentrant_getenv2 sync_signal1 pthread_atfork1 fork1 putenv_r1 \
	sizeof1 putenv_r2 putenv_r3 putenv_r4 putenv_r5 putenv_r6 putenv_r7 thr_sleep1 thr_sleep2

$(PROGS): %: %.o
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS) $(LDLIBS) 

recursive1: recursive1.o makethread1.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS) 

recursive2: recursive2.o makethread1.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS) 

clean:
	rm -vf *.o *~ $(PROGS)

.PHONY: clean1 

clean1:
	rm -vf *~

include $(ROOT)/Make.libmyapue.inc
