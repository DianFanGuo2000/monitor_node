
#include "can_fpu_recv_send.h"


// �ڳ����ʼ��ʱ������  
void initialize_can_fpu_lock() {  
    pthread_mutex_init(&can_fpu_lock, NULL);  
}  
  
// �ڳ������ʱ������  
void destroy_can_fpu_lock() {  
    pthread_mutex_destroy(&can_fpu_lock);  
}



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
	if(msg==NULL)
	{
		printf("[ERROR] receive_packet_can_fpu got a NULL msg!\n");
		return _ERROR;
	} 

	int len=-1;
	if(length>MAX_CAN_DATA_LENGTH)
		len = MAX_CAN_DATA_LENGTH;
	else
		len = length; 


    time_t now = time(NULL); 
	char data_frame[RECEIVED_CAN_DATA_PACKAGE_SIZE];
    while (time(NULL) - now < wait_time) // ���ﵥλΪ��  
    {  
    	pthread_mutex_lock(&can_fpu_lock);  //һ��ֻ����һ�������߳��ڵȴ�
        int ret = appCanDataRecv(can_channel_id, data_frame, RECEIVED_CAN_DATA_PACKAGE_SIZE, -1);// �������� //����78�Ƿ�������ʱ����װ��������֡���ֽڴ�С 
        pthread_mutex_unlock(&can_fpu_lock);  
        if (isAllZeros(data_frame,RECEIVED_CAN_DATA_PACKAGE_SIZE)) { 
            continue;  
        }else{  
			memcpy(msg, data_frame + OFFSET_OF_TARGET_DATA_IN_RECEIVED_CAN_DATA_PACKAGE, len);
            return _SUCCESS;  
        }
    }
	 
	printf("Can Recv Timeout\n");
    return _ERROR;  
}


int send_packet_can_fpu(UINT32 can_channel_id, const char *msg, UINT32 length) //�˴�len��������8��������ض�
{

	if(msg==NULL)
	{
		printf("[ERROR] send_packet_can_fpu got a NULL msg!\n");
		return _ERROR;
	}


	int len=-1;
	if(length>MAX_CAN_DATA_LENGTH)
		len = MAX_CAN_DATA_LENGTH;
	else
		len = length;
	
	int ret = appCanDataSend(can_channel_id,0x1ff,msg,len);// appCanDataSendFunc�����ã�MCU�ᱨ��֡����Ҳ���֮��� //�˴�len��������8
	if(ret<0)
		return _ERROR;
	return _SUCCESS;
}





