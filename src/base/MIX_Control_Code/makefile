TARGET=libcan.so
#CROSS_COMPILE=/home/cjy/bms/host/bin/aarch64-buildroot-linux-gnu-
#CROSS_COMPILE=aarch64-buildroot-linux-gnu-

DEST_DIR1=../lib
DEST_DIR2=../target

CC = $(CROSS_COMPILE)gcc
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump
NM = $(CROSS_COMPILE)nm
CPPFLAGS =
LDFLAGS =

# 7. set runtime cpu type: TRG_X86 | TRG_PPC | TRG_ARM | TRG_CORTEX | TRG_SH | TRG_MIPS
CFLAGS += -DTRG_ARM
# 64bit X86/ARM -> also set TRG_64BIT
CFLAGS += -DTRG_64BIT
CFLAGS += -DLINUX -DCDECL= -DDLL_DECL= -DHUGEPTR=
CFLAGS += -g -O2 -Wall -Wextra -Wno-unused

CPPFLAGS += -I$(abspath ./all_code/inc)
CPPFLAGS += -DOS_NO_C_LIB_DEFINES

COMPILE = $(CC) $(CPPFLAGS) $(CFLAGS) -c
COMPILESO =$(CC) -fPIC -c  $(CPPFLAGS) $(CFLAGS)
LINKCC = $(CC)  -lrt
LINKCCSO = $(CC) -shared
LINKAR = ar 

LIBA = 

SRCS := $(wildcard ./all_code/src/*.c)

DEPS := $(patsubst %.c,%.d,$(SRCS))
OBJS := $(patsubst %.c,%.o,$(SRCS))

$(TARGET):$(OBJS) $(DEPS)
	$(LINKCCSO) $(OBJS) $(LIBA) -Wl,-soname,$(TARGET) -o $(TARGET) $(LDFLAGS)
%.d:%.c      
	$(CC) -MM $(CPPFLAGS)  $(CFLAGS)  $< > $@
%.o:%.c
	$(COMPILESO) -o $@  $<		

all:$(TARGET)

.PHONY:install
install:
	install $(TARGET) $(DEST_DIR1)/$(TARGET)
	install $(TARGET) $(DEST_DIR2)/$(TARGET)

	
.PHONY:clean
clean:
	rm -f $(OBJS) $(DEPS) $(TARGET) *.dump *.map *.debug
	

