ROOT=../..
include $(ROOT)/Make.defines

PROGS = select_main poll_main cli_main
LIBMISC = libopen.a
OBJS = csopen.o client_handler.o loop_select.o \
	   buf_args.o cli_args.o request.o loop_poll.o

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
