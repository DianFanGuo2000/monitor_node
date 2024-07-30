#ifndef CAN_RECV_SEND_H  
#define CAN_RECV_SEND_H  
#include <stdio.h>  
#include <stdlib.h>  
  

#include "../configures.h"

/*
ע�⣬�õײ�ģ����ʹ�õ�ϵ�п⺯�����������ض���FPUģ����Եģ�������CANӲ�����ú�FPUģ���Ӳ�����ò�һ�£�������
����õײ�ģ���е���ش��룬��ȷ����ض���ӿں�����������������
*/

#ifndef UINT8
typedef unsigned char             UINT8;
#endif
#ifndef UINT16
typedef unsigned short            UINT16;
#endif
#ifndef UINT32
typedef unsigned int               UINT32;
#endif


#ifndef INT8
typedef signed char     INT8;
#endif
#ifndef INT16
typedef short           INT16;
#endif
#ifndef INT32
typedef int             INT32;
#endif

#define STATUS  int

extern int comCanSTDCfgInit(int channel_id,int buad_rate);
extern INT32 appCanDataSendFunc(UINT32 com,char *sendDataPointer,UINT32 sendDataLength );
extern INT32 appCanDataRecv(UINT32 com,char *recvDataPointer,UINT32 recvDataLength,INT32 Timeout_I32);

extern INT32 TASK_DELAY();



int receive_packet_can(UINT32 can_channel_id, char *msg, UINT32 length, int wait_time);

int send_packet_can(UINT32 can_channel_id, const char *msg, UINT32 length);


#endif
