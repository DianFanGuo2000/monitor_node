#ifdef __cplusplus  
extern "C" {  
#endif  
  
typedef int STATUS;  
typedef int INT32;  
typedef unsigned char UINT8;  
typedef unsigned short UINT16;  
typedef unsigned int UINT32;  
  
/* com ȡֵ 1��2,3,4��5,6��7,8 */  
/* mode 1-��׼֡;3-��չ֡ */  
/* bpsRate canͨ�Ų����� */  
STATUS comCfgInit(UINT32 com, UINT32 mode, UINT32 bpsRate); /* ͨ�����ó�ʼ�� */  
  
/* recvDataPointer ��Ž��յ���CAN���� */  
/* recvDataLength �������ݳ��� */  
INT32 appCanDataRecv(UINT32 com, char *recvDataPointer, UINT32 recvDataLength,INT32 Timeout_I32);  
  
/* dataPointer ��ŷ���CAN���� */  
/* dataLength �������ݳ��� */  
STATUS appCanDataSend(UINT32 com, UINT32 canId, UINT8 *dataPointer, UINT32 dataLength);  
  
/* recvDataPointer ��Ž��յ���CAN���� */  
/* recvDataLength �������ݳ��� */  
UINT32 appMcuStatRecv(char *recvDataPointer, UINT32 recvDataLength);  
  
#ifdef __cplusplus  
}  
#endif

