ROOT=..
include $(ROOT)/Make.defines

PROGS = seektest1 holefile1 sysconf1 copy_in_out1 holefile2 copy_in_out2 \
		pr_flags1 turnflags1 turnflags2 copy_in_out3 copy_in_out4 copy_in_out5 \
		copy_in_out6 copy_in_out7 exer6_1

SIMUDUP2 = simudup2_1 simudup2_2 simudup2_3

all:	clean1 $(PROGS) $(SIMUDUP2)

$(PROGS): %: %.o
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS) $(LDLIBS) 

$(SIMUDUP2): %: %.o get_fl.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS) 


clean:
	rm -vf *.o *~ $(PROGS)

.PHONY: clean1

clean1:
	rm -vf *~

include $(ROOT)/Make.libmyapue.inc
