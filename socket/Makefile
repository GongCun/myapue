ROOT=..
include $(ROOT)/Make.defines

PROGS = getaddrinfo1 uptime_client uptime_server gethostname1 \
		uptime_server2 uptime_client2 uptime_udp_client uptime_udp_server \
		uptime_server3 oob_send01 oob_recv01 oob_recv02 oob_recv03 oob_send02 \
		oob_send03 oob_recv04 oob_send04 aio_recv01 aio_send01 aio_send02 \
		aio_recv02 exer1 exer1_02 exer2_01 exer2_02 exer3_01 exer3_02 exer3_03 \
		exer3_04 exer3_05 exer4_serv_01 exer4_clit_01 exer5_01 exer5_02 exer5_02_client \
		exer5_03 exer6_naio_recv exer5_04 exer5_05 exer3_06

all:	clean1 $(PROGS) 

$(PROGS): %: %.o
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS) $(LDLIBS) 

clean:
	rm -vf *.o *~ $(PROGS)

.PHONY: clean1

clean1:
	rm -vf *~

include $(ROOT)/Make.libmyapue.inc
