ROOT=..
include $(ROOT)/Make.defines

PROGS = daemon1 daemon2 daemon3 getlogin1 daemon4 daemonsvr
LIBMISC = libdaemon.a
OBJS   = daemonize.o already_running.o

all:	clean1 $(LIBMISC) $(PROGS) daemonize.o already_running.o

$(LIBMISC): $(OBJS)
	$(AR) rv $(LIBMISC) $?
	$(RANLIB) $(LIBMISC)

$(PROGS): %: %.o
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS) $(LDLIBS) -L. -ldaemon

daemonize.o: daemonize.c
	$(CC) $(CFLAGS) $< -c

already_running.o: already_running.c
	$(CC) $(CFLAGS) $< -c

clean:
	rm -vf *.o *~ $(PROGS)

.PHONY: clean1

clean1:
	rm -vf *~

include $(ROOT)/Make.libmyapue.inc
