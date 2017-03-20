ROOT=..
include $(ROOT)/Make.defines

PROGS = scopy_in_out1 scopy_in_out2 scopy_in_out3 bufferio1 bufferinfo1 \
		tmpfile1 tempnam1 mkstemp1 setbuf1 scopy_in_out4 exer4 tempnam2 \
		scopy_in_out5 mktempfile

all:	clean1 $(PROGS)

$(PROGS): %: %.o
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS) $(LDLIBS) 

clean:
	rm -vf *.o *~ $(PROGS)

.PHONY: clean1

clean1:
	rm -vf *~

include $(ROOT)/Make.libmyapue.inc
