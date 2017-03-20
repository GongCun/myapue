ROOT=../..
include $(ROOT)/Make.defines

PROGS = driver

all:	clean1 pty $(PROGS)

$(PROGS): %: %.o
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS) $(LDLIBS)

pty: pty_main.o loop.o do_driver.o
	$(CC) $(CFLAGS) pty_main.o loop.o do_driver.o -o $@ $(LDFLAGS) $(LDLIBS)

clean:
	rm -vf *.o *~ $(PROGS)

.PHONY: clean1

clean1:
	rm -vf *~

include $(ROOT)/Make.libmyapue.inc
