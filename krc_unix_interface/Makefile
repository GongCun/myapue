ROOT=..
include $(ROOT)/Make.defines

PROGS = main

LIBMISC = libsyscalls.a

OBJS = krfopen.o _fillbuf.o _flushbuf.o xfflush.o krfclose.o krfseek.o \
	   krmalloc.o krcalloc.o krmalloc_safe.o bfree.o

all:	clean1 $(LIBMISC) $(OBJS) $(PROGS)

$(LIBMISC): $(OBJS)
	$(AR) rv $(LIBMISC) $?
	$(RANLIB) $(LIBMISC)

$(PROGS): %: %.o
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS) $(LDLIBS) -L. -lsyscalls -I.

clean:
	rm -vf *.o *~

.PHONY: clean1

clean1:
	rm -vf *~

include $(ROOT)/Make.libmyapue.inc
