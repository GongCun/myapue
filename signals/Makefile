ROOT=..
include $(ROOT)/Make.defines

PROGS = test signal1 getpwnam1 nonreentrant1 alarm1 nonreentrant2 alarm2 alarm3 msignal2 alarm4 sigsuspend2 sigsuspend3 \
		csystem1 sig_chld1 csystem4 forkt1 forkt2 forkt3 forkt4 forkt5 forkt6 forkt7 sigtstp1 \
		exer1 exer2 exer5 writet1 sigblock1 sigblock2 exer6_1 exer6_2 exer6_3 exer9_1 exer10_1 exer10_2 exer11_1 \
		exer11_2 exer11_3 exer12_1 exer12_2 exer12_3 exer12_4 exer12_5 exer12_6 exer12_7 exer12_8 exer12_9 \
		exer12_10 exer5_2

all:	$(PROGS) clean1 msleep msleep1 signal2.o msignal1 signal3_1.o sigaction1.o alarm5 alarm6 sigjmp1 sigjmp2 \
	sigsuspend1 mabort1 mabort2 mabort3 csystem2 csystem3 csystem5 tsys csleep3 exer9_2

$(PROGS): %: %.o
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS) $(LDLIBS)

msleep: msleep.o sleep1.o sleep2.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)

msleep1: msleep1.o sleep2.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)

csleep3: csleep3.o sleep3.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)

msignal1: msignal1.o signal3.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)

alarm5: alarm5.o sigaction1.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)

alarm6: alarm6.o sigaction1.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)

sigjmp1: sigjmp1.o sigaction1.o signal3_1.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)

sigjmp2: sigjmp2.o signal3_1.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)

sigsuspend1: sigsuspend1.o signal3_1.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)

mabort1: mabort1.o sigaction1.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)

csystem2: csystem2.o signal3_1.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)

csystem3: csystem3.o ystem.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)

csystem5: csystem5.o ystem.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)

tsys: tsys.o pr_exit.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)

exer9_2: exer9_2.o exer9_pr_mask.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)

clean:
	rm -vf *.o *~ $(PROGS)

.PHONY: clean1

clean1:
	rm -vf *~

include $(ROOT)/Make.libmyapue.inc
