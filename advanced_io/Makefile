ROOT=..
include $(ROOT)/Make.defines

PROGS = lnonblkwr1 deadlock1 determine1 select1 poll1 poll2 open_tty \
		asyncio1 asyncio2 rdwrv1 rdwrv2 rdwrv3 rdwrn1 rdwrn2 asyncio3 \
		mmap1 convio1 asyncio4 rdwrlock1 rdwrlock2 rdwrlock3 sleep_us1 \
		sleep_us2 sleep_us3 pipe_max mmap2 mmap3 rdwrv4 rdwrv5 rdwrv6 \
		rdwrv7 asyncio5 asyncio6 pipe_max2 asyncio7

all:	clean1 $(PROGS)

$(PROGS): %: %.o
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS) $(LDLIBS) -lcurses

clean:
	rm -vf *.o *~ $(PROGS)

.PHONY: clean1

clean1:
	rm -vf *~

include $(ROOT)/Make.libmyapue.inc
