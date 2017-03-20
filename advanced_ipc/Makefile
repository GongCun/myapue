ROOT=..
include $(ROOT)/Make.defines

PROGS = un_sock_bind un_sock_bind02 cli_main01 svr_main01 \
		socketpair01 socketpair02 fd_recv01 fd_send01 \
		exer2 exer2_v2
LIBMISC = libopen.a
OBJS = csopen01.o request01.o buf_args.o cli_args.o

all:	clean1 $(LIBMISC) $(OBJS) $(PROGS)

$(PROGS): %: %.o
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS) $(LDLIBS) -L. -lopen -I.

$(LIBMISC): $(OBJS)
	$(AR) rv $(LIBMISC) $?
	$(RANLIB) $(LIBMISC)

clean:
	rm -vf *.o *~ $(PROGS)

.PHONY: clean1

clean1:
	rm -vf *~

include $(ROOT)/Make.libmyapue.inc
