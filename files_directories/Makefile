ROOT=..
include $(ROOT)/Make.defines

PROGS = pr_type1 access1 umask1 set_uid_test set_uid_test2 set_uid_test3 \
		set_uid_test4 chmod1 chmod2 chown1 chown2 ftruncate1 unlink1 unlink2 \
		link1 sym_rd_link1 utime1 mkdir1 rmdir1 ls1 ftw1 ftw2 ftw3 ftw4 \
		chdir1 pathmax1 getcwd1 st_dev1 exer1 exer2 exer3 exer5 mycp1 mycp2 \
		exer9 ftw5 exer13 exer13_2 exer16 exer16_2 exer16_3 exer17 scopy_in_out1 \
		scopy_in_out2 scopy_in_out3 bufferio1 bufferinfo1

all:	clean1 $(PROGS)

$(PROGS): %: %.o
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS) $(LDLIBS) 

clean:
	rm -vf *.o *~ $(PROGS)

.PHONY: clean1

clean1:
	rm -vf *~

include $(ROOT)/Make.libmyapue.inc
