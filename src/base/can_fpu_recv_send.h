#ifndef CAN_RECV_SEND_H  
#define CAN_RECV_SEND_H  
#include <stdio.h>  
#include <stdlib.h>  
  


#include "../configures.h"

 

/* 
ע�⣬�õײ�ģ����ʹ�õ�ϵ�п⺯�����������ض���FPUģ����Եģ�������CANӲ�����ú�FPUģ���Ӳ�����ò�һ�£�������
����õײ�ģ���е���ش��룬��ȷ����ض���ӿں�����������������
*/
#include "can_interface.h"
#include <pthread.h>  


pthread_mutex_t can_fpu_lock;

// �ڳ����ʼ��ʱ������  
void initialize_can_fpu_lock();
  
// �ڳ������ʱ������  
void destroy_can_fpu_lock();


extern int comCanSTDCfgInit(int channel_id,int buad_rate);
extern INT32 appCanDataSendFunc(UINT32 com,char *sendDataPointer,UINT32 sendDataLength );
extern INT32 appCanDataRecv(UINT32 com,char *recvDataPointer,UINT32 recvDataLength,INT32 Timeout_I32);

extern INT32 TASK_DELAY();



int receive_packet_can_fpu(UINT32 can_channel_id, char *msg, UINT32 length, int wait_time);

int send_packet_can_fpu(UINT32 can_channel_id, const char *msg, UINT32 length);


#endif
