#ifndef CAN_GPU_RECV_SEND_H  
#define CAN_GPU_RECV_SEND_H  

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <net/if.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/can.h> 
#include <linux/can/raw.h>
#include "../configures.h"
#include <pthread.h>  


#define CAN_ID      0x123
#define NAME0 "can0"
#define NAME1 "can1"

pthread_mutex_t can_gpu_lock;

// 在程序初始化时创建锁  
void initialize_can_gpu_lock();
  
// 在程序结束时销毁锁  
void destroy_can_gpu_lock();


int send_packet_can_gpu(int can_id, const char *msg, int len);
int receive_packet_can_gpu(int can_id, unsigned char *msg, int length, long wait_time); 
int can0Recv(int len, long wait_time);
int can1Recv(int len, long wait_time);
int isAllZero(char arr[], int size);

#endif


