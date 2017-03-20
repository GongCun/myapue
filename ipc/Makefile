ROOT=..
include $(ROOT)/Make.defines

PROGS = ptoc1 pager1 pcsync pager2 pager3 permission clt_svr \
		myuclc filter add2 drive add2_no add2_yes multisvr multiclt \
		multiclt2 exer17 exer1 exer2 exer3 exer4 exer5 exer6 exer7 \
		exer7_2 exer7_poll exer7_poll_2 exer7_3 exer7_sig exer7_poll_sig \
		exer7_sel_wr exer7_po_wr exer8 exer10 exer10_2 exer10_3 exer17_2

all:	clean1 $(PROGS) exer16 exer16_2

$(PROGS): %: %.o
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS) $(LDLIBS) 

exer16: exer16.o mylock.o
	$(CC) $(CFLAGS) exer16.o mylock.o -o $@ $(LDFLAGS) $(LDLIBS) 

exer16_2: exer16_2.o sem_opt.o
	$(CC) $(CFLAGS) exer16_2.o sem_opt.o -o $@ $(LDFLAGS) $(LDLIBS) 

clean:
	rm -vf *.o *~ $(PROGS)

.PHONY: clean1

clean1:
	rm -vf *~

include $(ROOT)/Make.libmyapue.inc
