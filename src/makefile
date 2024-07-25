# 定义编译器和编译选项  
CC=gcc  
CFLAGS=-I. -Wall  
LDFLAGS=-L/usr/local/lib  
  
# 定义源文件和目标文件（包括当前目录和base目录下的.c文件）  
SRCS=$(wildcard *.c)  
BASE_SRCS=$(wildcard base/*.c)  
OBJS=$(SRCS:.c=.o)  
BASE_OBJS=$(BASE_SRCS:.c=.o)  
TARGET=myapp  
  
# 依赖的库  
LIBS=-lpthread -lcjson 
  
# 默认目标  
all: $(TARGET)  
  
# 编译每个源文件（包括base/目录下的）  
%.o: %.c  
	$(CC) $(CFLAGS) -c $< -o $@  
  
# 主目标，依赖于所有.o文件  
$(TARGET): $(OBJS) $(BASE_OBJS)  
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@ $(LIBS)  
  
# 清理编译生成的文件  
clean:  
	@echo "Cleaning up current directory..."  
	rm -f $(OBJS) $(TARGET)  
	@echo "Cleaning up base directory..."  
	$(MAKE) -C base clean
  
.PHONY: all clean