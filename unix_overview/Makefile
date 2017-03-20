ROOT=..
include $(ROOT)/Make.defines

PROGS = pr_pid exer5

all:	clean1 $(PROGS) 

$(PROGS): %: %.o
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS) $(LDLIBS)

clean:
	rm -vf *.o *~ $(PROGS)

.PHONY: clean1

clean1:
	rm -vf *~

include $(ROOT)/Make.libmyapue.inc
