#
ROOT=..
include $(ROOT)/Make.defines

LIBMISC = libmyapue.a
OBJS   = error.o tellwait.o pathalloc.o set_fl.o clr_fl.o get_fl.o \
		 lockreg.o locktest.o readn.o writen.o setticker.o ptellwait.o \
		 openmax.o mpipe_open_close.o readline.o px_ipc_name.o connect_retry.o \
		 initserver.o daemonize.o reinitserver.o tcp_connect.o tcp_listen.o \
		 signal_rest.o tcp_bind.o s_pipe.o serv_listen.o serv_accept.o cli_connect.o \
		 send_err.o send_fd.o recv_fd.o log.o tty_noncanon.o pty_open.o pty_fork.o \
		 signal_intr.o strregex.o

all:	$(LIBMISC) clean1

$(LIBMISC): $(OBJS)
	$(AR) rv $(LIBMISC) $?
	$(RANLIB) $(LIBMISC)

clean:
	rm -vf *.o a.out core temp.* *~ $(LIBMISC)

.PHONY: clean1

clean1:
	rm -vf *~

include $(ROOT)/Make.libmyapue.inc
