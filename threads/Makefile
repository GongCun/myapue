ROOT=..
include $(ROOT)/Make.defines

PROGS = pr_thread1 pr_thread2 thr_term1 err_pthr_exit1 pthr_join1 pthr_join2 err_pthr_exit2 err_pthr_exit3 \
		pthr_clean1 pthr_clean2 pthr_clean3 mutex2 twomutexes2 condition2 rwlock2

all:	$(PROGS) clean1

$(PROGS): %: %.o
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS) $(LDLIBS) 

clean:
	rm -vf *.o *~ $(PROGS)

.PHONY: clean1

clean1:
	rm -vf *~

include $(ROOT)/Make.libmyapue.inc
