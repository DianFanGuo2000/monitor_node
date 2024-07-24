#CROSS_COMPILE=aarch64-linux-gnu-
CC = $(CROSS_COMPILE)gcc
CFLAGS=-I. -Wall  
LDFLAGS=-L /usr/local/lib  
  
SRCS=$(wildcard *.c)  
BASE_SRCS=$(wildcard base/*.c)  
OBJS=$(SRCS:.c=.o)  
BASE_OBJS=$(BASE_SRCS:.c=.o)  
TARGET=myapp

LIBS= -lpthread -lcjson 

all: $(TARGET)  

%.o: %.c  
	$(CC) $(CFLAGS) -c $< -o $@  

$(TARGET): $(OBJS) $(BASE_OBJS)  
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@ $(LIBS)  

clean:  
	@echo "Cleaning up current directory..."  
	rm -f $(OBJS) $(TARGET)  
	@echo "Cleaning up base directory..."  
	$(MAKE) -C base clean
  
.PHONY: all clean