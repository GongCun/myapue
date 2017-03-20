ROOT=../..
include $(ROOT)/Make.defines

PROGS = exer8_1 exer8_2 exer8_2_1 exer8_2_2 4_1 5_1 5_2 6_1 7_1 7_4 \
		pr_flags1 7_5 7_6

PRPROGS = exer8_1_1 3_1 3_2 

all:	$(PROGS) $(PRPROGS) clean1 exec1 7_2 7_3 exec2

$(PROGS): %: %.o
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS) $(LDLIBS)

$(PRPROGS):	%: %.o pr_exit.o
	$(CC) $(CFLAGS) $< pr_exit.o -o $@ $(LDFLAGS) $(LDLIBS)

exec1: exec1.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)

exec2: exec2.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)

7_2: 7_2.o dir.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)

7_3: 7_3.o dir.o
	$(CC) -ggdb $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)

clean:
	rm -vf *.o *~ $(PROGS)

.PHONY: clean1

clean1:
	rm -vf *~

include $(ROOT)/Make.libmyapue.inc
