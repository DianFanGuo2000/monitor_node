 CROSS_COMPILE=aarch64-linux-gnu-
CC = $(CROSS_COMPILE)gcc

CFLAGS=-I.  

LDFLAGS=  

OBJECTS=eth_recv_send.o rs485_recv_send.o can_recv_send.o  

all: $(OBJECTS)  

%.o: %.c  
	$(CC) $(CFLAGS) -c -o $@ $<  

clean:  
	rm -f $(OBJECTS)  

eth_recv_send.o: eth_recv_send.c eth_recv_send.h base_configures.h  
rs485_recv_send.o: rs485_recv_send.c rs485_recv_send.h base_configures.h  
can_recv_send.o: can_recv_send.c can_recv_send.h base_configures.h