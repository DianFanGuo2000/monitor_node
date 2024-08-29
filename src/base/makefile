# 定义编译器  
CC=gcc  
  
# 定义编译选项  
CFLAGS=-I.  
  
# 定义链接选项（这里留空）  
LDFLAGS=  
  
# 使用自动变量来获取当前目录下的所有.c文件，并为它们生成.o文件  
SOURCES=$(wildcard *.c)  
OBJECTS=$(SOURCES:.c=.o)  
  
# 默认目标  
all: $(OBJECTS)  
  
# 编译C源文件生成目标文件  
%.o: %.c  
	$(CC) $(CFLAGS) -c -o $@ $<  
  
# 清理编译生成的文件  
clean:  
	rm -f $(OBJECTS)