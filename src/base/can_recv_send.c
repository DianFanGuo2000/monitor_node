
#include "can_recv_send.h"

int isAllZeros(char arr[], int size) {  
    for (int i = 0; i < size; i++) {  
        if (arr[i] != 0) {  
            return 0;  
        }  
    }  
    return 1;  
}  
  


int receive_packet_can_fpu(UINT32 can_channel_id, char *msg, UINT32 length, int wait_time)  
{  

	int len=-1;
	if(length>MAX_CAN_DATA_LENGTH)
		len = MAX_CAN_DATA_LENGTH;
	else
		len = length;


    time_t now = time(NULL); 
	char data_frame[RECEIVED_CAN_DATA_PACKAGE_SIZE];
    while (time(NULL) - now < wait_time) // 这里单位为秒  
    {  
        int ret = appCanDataRecv(can_channel_id, data_frame, RECEIVED_CAN_DATA_PACKAGE_SIZE, -1); //这里78是发送数据时，包装数据所得帧的字节大小 
        if (isAllZeros(data_frame,RECEIVED_CAN_DATA_PACKAGE_SIZE))  
            continue;  
        else  
			memcpy(msg, data_frame + OFFSET_OF_TARGET_DATA_IN_RECEIVED_CAN_DATA_PACKAGE, len);
            return _SUCCESS;  
    }  
    return _ERROR;  
}


int send_packet_can_fpu(UINT32 can_channel_id, const char *msg, UINT32 length) //此处len不允许超过8，超过会截断
{
	int len=-1;
	if(length>MAX_CAN_DATA_LENGTH)
		len = MAX_CAN_DATA_LENGTH;
	else
		len = length;
	
	int ret = appCanDataSend(can_channel_id,0x1ff,msg,len);// appCanDataSendFunc不能用，MCU会报错，帧序号找不到之类的 //此处len不允许超过8
	if(ret<0)
		return _ERROR;
	return _SUCCESS;
}





