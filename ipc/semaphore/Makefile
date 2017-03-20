ROOT=../..
include $(ROOT)/Make.defines

PROGS = semcreate semclose semunlink semwait sempost \
		prod_con prod_con_deadlock prod_con_deadlock2 \
		semlockfile

all:	clean1 $(PROGS)

$(PROGS): %: %.o
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS) $(LDLIBS)

clean:
	rm -vf *.o *~ $(PROGS)

.PHONY: clean1

clean1:
	rm -vf *~

include $(ROOT)/Make.libmyapue.inc
