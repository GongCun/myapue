ROOT=..
include $(ROOT)/Make.defines

PROGS = change01 termopt01 baud01 baud02 linectl01 \
		linectl02 ctermid01 ctermid02 ctermid03 \
		isatty01 isatty02 ttyname01 ttyname02 getpass01 \
		getpass02 noncanon01 noncanon02 cread01 pr_win01 \
		exer1_01

all:	clean1 $(LIBMISC) $(OBJS) $(PROGS)

$(PROGS): %: %.o
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS) $(LDLIBS)

clean:
	rm -vf *.o *~ $(PROGS)

.PHONY: clean1

clean1:
	rm -vf *~

include $(ROOT)/Make.libmyapue.inc
