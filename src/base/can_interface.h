#ifdef __cplusplus  
extern "C" {  
#endif  
  
typedef int STATUS;  
typedef int INT32;  
typedef unsigned char UINT8;  
typedef unsigned short UINT16;  
typedef unsigned int UINT32;  
  
/* com 取值 1，2,3,4，5,6，7,8 */  
/* mode 1-标准帧;3-扩展帧 */  
/* bpsRate can通信波特率 */  
STATUS comCfgInit(UINT32 com, UINT32 mode, UINT32 bpsRate); /* 通信配置初始化 */  
  
/* recvDataPointer 存放接收到的CAN数据 */  
/* recvDataLength 接收数据长度 */  
INT32 appCanDataRecv(UINT32 com, char *recvDataPointer, UINT32 recvDataLength,INT32 Timeout_I32);  
  
/* dataPointer 存放发送CAN数据 */  
/* dataLength 发送数据长度 */  
STATUS appCanDataSend(UINT32 com, UINT32 canId, UINT8 *dataPointer, UINT32 dataLength);  
  
/* recvDataPointer 存放接收到的CAN数据 */  
/* recvDataLength 接收数据长度 */  
UINT32 appMcuStatRecv(char *recvDataPointer, UINT32 recvDataLength);  
  
#ifdef __cplusplus  
}  
#endif

