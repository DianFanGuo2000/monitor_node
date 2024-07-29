
#include "firmware.h"





// 线程函数，用于异步处理消息  
void* deal_async(void* arg) {  
    DealData* data = (DealData*)arg;  
    data->deal_func(data->linked_node,data->listened_interface,data->msg); // 调用 deal 函数处理消息  
}  


int receive_message(const char *linked_node,const char *source_interface,Dealer deal,long max_waiting_time)    
{

	char *interface_status = get_interface_status(source_interface);
	if(strcmp(interface_status, "sending") == 0 || strcmp(interface_status, "closed") == 0 )
	{
		return _ERROR; /*如果是单纯sending或closed，那么不做任何接收*/
	}


	char *interface_type = get_interface_type(source_interface);
	time_t begin_time = time(NULL); // Initialize start time  

    // Check if the interface type is "eth"  
    if (strcmp(interface_type, "eth") == 0) {    
	    // Declare a buffer for the received message, assuming it might be longer  
		DealData data;          
		data.deal_func = deal; // 将 deal 函数指针保存到结构体中
		data.linked_node = linked_node;
		data.listened_interface = source_interface;
		
	    // Attempt to receive a packet from the source interface  
	    if (receive_packet(source_interface, data.msg, max_waiting_time)<0) {    
	        // If receiving the message fails, return an error and don't reply
	        return _ERROR;    
	    }    


        pthread_t thread_id;  
        if (pthread_create(&thread_id, NULL, deal_async, &data) != 0) {  
            // 线程创建失败处理，返回错误  
            return _ERROR;  
        }  

	    return _SUCCESS;    
    }


	
	if (strcmp(interface_type, "rs485") == 0) {	 
			// Declare a buffer for the received message, assuming it might be longer  
			DealData data;			
			data.deal_func = deal; // 将 deal 函数指针保存到结构体中
			data.linked_node = linked_node;
			data.listened_interface = source_interface;

			SerialPortParams params;
			int index = get_interface_index(source_interface);
			params.baudrate = get_baud_rate_by_index(index);
			params.databits = get_databits_by_index(index);
			params.stopbits = get_stopbits_by_index(index);
			params.paritybits = get_paritybits_by_index(index);
			
			int fd = open_port(source_interface, 0, params);

			// Attempt to receive a packet from the source interface  
			if (receive_packet_rs485(fd,data.msg,RS485_LEN,max_waiting_time)<0) {	 
				// If receiving the message fails, return an error and don't reply
				return _ERROR;	  
			}	 
	
	
			pthread_t thread_id;  
			if (pthread_create(&thread_id, NULL, deal_async, &data) != 0) {  
				// 线程创建失败处理，返回错误  
				return _ERROR;	
			}  

			close_port(fd);
	
			return _SUCCESS;	
	}	 

}


void fillMessageToRS485Len(const char *message, char *RS485MSG, int rs485_len) {  
    // 首先，将原始消息复制到 RS485MSG  
    strncpy(RS485MSG, message, rs485_len);  
	
      
    // 如果原始消息长度大于 RS485_LEN，截断它  
    if (strlen(message) > rs485_len) {  
        RS485MSG[rs485_len] = '\0'; // 确保字符串正确终止  
    } else {  
        // 否则，用空格填充剩余部分  
        size_t len = strlen(RS485MSG);  
        memset(RS485MSG + len, ' ', rs485_len - len);  
        // 确保字符串在末尾正确终止  
        RS485MSG[rs485_len] = '\0';  
    }  
      
    // 可选：打印结果以验证  
    printf("Filled message: '%s'\n", RS485MSG);  
}  


int send_message(const char *source_interface,const char *message)
{   

	char *interface_status = get_interface_status(source_interface);
	if(strcmp(interface_status, "receiving") == 0 || strcmp(interface_status, "closed") == 0 )
	{
		return _ERROR; /*如果是单纯receiving或closed，那么不做任何发送*/
	}

	char *interface_type = get_interface_type(source_interface);
	char msg[MAX_MSG_LEN]; // Buffer for the received message      
    time_t begin_time = time(NULL); // Initialize start time  
  
    // Check if the interface type is "eth" (could be removed if not needed)    
    if (strcmp(interface_type, "eth") == 0) {    
        // Retrieve the MAC addresses    
        const char *src_mac = get_mac_addr(source_interface);    
        const char *dest_mac = get_linked_mac_addr(source_interface);  
        // Check if MAC addresses were retrieved and packet can be sent    
        if (send_packet(source_interface, message, src_mac, dest_mac) < 0) {    
        	printf("send failed!\n");
        	return _ERROR; // Retry sending    
        }    
		return _SUCCESS;
    }  


    // Check if the interface type is "rs485" (could be removed if not needed)    
    if (strcmp(interface_type, "rs485") == 0) {    
		SerialPortParams params;
		int index = get_interface_index(source_interface);
		params.baudrate = get_baud_rate_by_index(index);
		params.databits = get_databits_by_index(index);
		params.stopbits = get_stopbits_by_index(index);
		params.paritybits = get_paritybits_by_index(index);
		
		int fd = open_port(source_interface, 0, params);

		// 填充至RS485_LEN，以保证发送长度一定是RS485_LEN个字节
		char RS485MSG[RS485_LEN + 1];
		fillMessageToRS485Len(message,RS485MSG,RS485_LEN);

        if (send_packet_rs485(fd, RS485MSG, RS485_LEN) < 0) {    
        	printf("send failed!\n");
        	return _ERROR; // Retry sending    
        }    
		return _SUCCESS;
    }  


	
  
    // This point should not be reached due to the infinite loop, but for completeness    
    return _ERROR; // In case of unexpected termination    
}





void set_status(const char *source_interface,const char *status)
{
	//status_array; // 先判断status在不在这个数组里面
	//status = sending receiving sending_and_receiving closed
	//这个先不写
	//这个不能单纯只是设置接口状态数据库中的status字段，而应该真正调用底层接口设置接口的状态
	//先判断是全双工还是半双工，如果是半双工，全部都合法，如果是全双工，sending receiving这两个输入不合法
}



void sync_status(const char *source_interface)
{
	//status = sending receiving sending_and_receiving closed
	//这个先不写
	//这个不能单纯只是返回接口状态数据库中的status字段，而应该真正调用底层接口去检查接口的状态，并且把检查结果同步到数据库
	//这个在start_info之后，载入数组后要启动一下
}




