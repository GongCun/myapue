ROOT=..
include $(ROOT)/Make.defines

PROGS = wait1 fork3 fork3_1 race1 race2 echoall exec1 echoarg exec2 exec2_1 systest systest1 systest2 \
		printuids printacct acct1 nice1 time1

LIBPROC = libproc.a

OBJS = system1.o

all:	$(PROGS) $(LIBPROC) err_test clean1

$(LIBPROC): $(OBJS)
	$(AR) rv $(LIBPROC) $?
	$(RANLIB) $(LIBPROC)

echoarg: echoarg.o
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS) $(LDLIBS)

exec1: exec1.o $(LIBMYAPUE)
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS) $(LDLIBS)

exec2: exec2.o $(LIBMYAPUE)
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS) $(LDLIBS)

exec2_1: exec2_1.o $(LIBMYAPUE)
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS) $(LDLIBS)

echoall: echoall.o
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS) $(LDLIBS)

race1: race1.o $(LIBMYAPUE)
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS) $(LDLIBS)

race2: race2.o $(LIBMYAPUE)
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS) $(LDLIBS)

err_test: err_test.o $(LIBMYAPUE)
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS) $(LDLIBS)

fork3:	fork3.o $(LIBMYAPUE)
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS) $(LDLIBS)

fork3_1: fork3_1.o $(LIBMYAPUE)
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS) $(LDLIBS)

wait1:	wait1.o pr_exit.o $(LIBMYAPUE)
	$(CC) $(CFLAGS) wait1.o pr_exit.o -o wait1 $(LDFLAGS) $(LDLIBS)

systest:	systest.o pr_exit.o $(LIBMYAPUE) $(LIBPROC)
	$(CC) $(CFLAGS) systest.o pr_exit.o -o $@ $(LDFLAGS) $(LDLIBS) -L. -I. -lproc

systest1:	systest1.o pr_exit.o $(LIBMYAPUE)
	$(CC) $(CFLAGS) systest1.o pr_exit.o -o $@ $(LDFLAGS) $(LDLIBS)

systest2:	systest2.o pr_exit.o
	$(CC) $(CFLAGS) systest2.o pr_exit.o -o $@ $(LDFLAGS) $(LDLIBS) 

printuids:	printuids.o $(LIBMYAPUE)
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS) $(LDLIBS)

printacct:	printacct.o $(LIBMYAPUE)
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS) $(LDLIBS)

acct1:	acct1.o $(LIBMYAPUE)
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS) $(LDLIBS)

nice1:	nice1.o $(LIBMYAPUE)
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS) $(LDLIBS)

time1:	time1.o pr_exit.o $(LIBMYAPUE)
	$(CC) $(CFLAGS) time1.o pr_exit.o -o $@ $(LDFLAGS) $(LDLIBS)

clean:
	rm -vf *.o *~ $(PROGS)

.PHONY: clean1

clean1:
	rm -vf *~

include $(ROOT)/Make.libmyapue.inc
