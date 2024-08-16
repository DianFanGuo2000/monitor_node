#ifndef CAN_RECV_SEND_H  
#define CAN_RECV_SEND_H  
#include <stdio.h>  
#include <stdlib.h>  
  


#include "../configures.h"

 

/* 
注意，该底层模块所使用的系列库函数基本上是特定于FPU模块而言的，如果你的CAN硬件配置和FPU模块的硬件配置不一致，请自行
适配该底层模块中的相关代码，以确保相关对外接口函数可以正常工作。
*/
#include "can_interface.h"
#include <pthread.h>  


pthread_mutex_t can_fpu_lock;

// 在程序初始化时创建锁  
void initialize_can_fpu_lock();
  
// 在程序结束时销毁锁  
void destroy_can_fpu_lock();


extern int comCanSTDCfgInit(int channel_id,int buad_rate);
extern INT32 appCanDataSendFunc(UINT32 com,char *sendDataPointer,UINT32 sendDataLength );
extern INT32 appCanDataRecv(UINT32 com,char *recvDataPointer,UINT32 recvDataLength,INT32 Timeout_I32);

extern INT32 TASK_DELAY();



int receive_packet_can_fpu(UINT32 can_channel_id, char *msg, UINT32 length, int wait_time);

int send_packet_can_fpu(UINT32 can_channel_id, const char *msg, UINT32 length);


#endif
