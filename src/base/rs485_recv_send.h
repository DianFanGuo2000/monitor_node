#ifndef RS485_RECV_SEND_H  
#define RS485_RECV_SEND_H  


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <asm/ioctls.h>
#include <stdio.h>
#include <linux/serial.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <errno.h>
#include "../configures.h"

#define TIOCSRS485START	0x5430
#define TIOCSRS485STOP 	0x5431
#include <pthread.h>  


pthread_mutex_t rs485_lock;
// 在程序初始化时创建锁  
void initialize_rs485_lock();
// 在程序结束时销毁锁  
void destroy_rs485_lock();




typedef struct _SerialPortParams
{
    int baudrate;
    int databits;
    int stopbits;
    unsigned char paritybits;
} SerialPortParams;

void exportGPIO(int rs485_gpio_number);


int set_rs485(int fd);
int openPortRaw(const char* port, int openMode);
int setPortAttr(int fd, struct termios* portAttr);
int setBaudrate(int fd, struct termios* portAttr, int baudrate);
int setDatabits(struct termios* portAttr, int databits);
int setParity(struct termios* portAttr, unsigned char paritybits);
int setStopbits(struct termios* portAttr, int stopbits);

int open_port(const char* port, int openMode, SerialPortParams params);
int serialPortRead(int fd, unsigned char* buff, unsigned int length);
int serialPortWrite(int fd, unsigned char* buff, unsigned int length);
int close_port(int fd);

int send_packet_rs485(int fd, const char *msg, unsigned int length);
int receive_packet_rs485(int fd, unsigned char *msg, unsigned int length, int wait_time);




#endif
