
#include "firmware.h"



int init_basic_interface(int i)  
{  
    char *interface_name = get_interface_name_by_index(i);  
    char *interface_type = get_interface_type_by_index(i); 

	printf("%s being initialized\n",interface_name);
  
    if (interface_name == NULL || interface_type == NULL) {  
        // Handle NULL pointers appropriately  
        return _ERROR; // Or some other error code  
    }  
  
    if (strcmp(interface_type, "eth") == 0)  
    {  
        char* ip_addr = get_ip_addr_by_index(i);  
        char* mask = get_net_mask_by_index(i);  
          
        if (ip_addr == NULL || mask == NULL) {  
            // Handle NULL pointers appropriately  
            return _ERROR; // Or some other error code  
        }  

		set_interface_status(interface_name,"sending_and_receiving");
  
        // Use snprintf or similar to safely format strings for system call  
        char cmd_up[256];  
        snprintf(cmd_up, sizeof(cmd_up), "ifconfig %s up", interface_name);  
        system(cmd_up); 
		
  
        char cmd_addr[256];  
        snprintf(cmd_addr, sizeof(cmd_addr), "ifconfig %s %s netmask %s", interface_name, ip_addr, mask);  
        system(cmd_addr);  

		printf("interface_name:%s, ip_addr:%s, mask:%s\n",interface_name,ip_addr,mask);
    }  
  
    if (strcmp(interface_type, "rs485") == 0)  
    {  
		SerialPortParams params;
		params.baudrate = get_baud_rate_by_index(i);
		params.databits = get_databits_by_index(i);
		params.stopbits = get_stopbits_by_index(i);
		params.paritybits = get_paritybits_by_index(i);
			
		int fd = open_port(interface_name, 0, params);
		set_temporary_fd(i,fd);
	
        int rs485_gpio_number = get_rs485_gpio_number_by_index(i);  
        exportGPIO(rs485_gpio_number); // Assuming exportGPIO is defined elsewhere  
		set485RX();
		set_interface_status(interface_name,"receiving");
    }  

    if (strcmp(interface_type, "can") == 0)  
    {  
        int channel_id = get_channel_id_by_index(i);  
		int baud_rate = get_baud_rate_by_index(i);
		if(comCanSTDCfgInit(channel_id,baud_rate)<0)
		{
			set_interface_status(interface_name,"closed");
			return _ERROR;
		}
		set_interface_status(interface_name,"receiving");
    }  
  
    // Free dynamically allocated strings if necessary (not shown here)  
  
    return _SUCCESS; // Success  
}  


// Function to close a basic interface based on its type  
int close_basic_interface(int i)  
{  
    char *interface_name = get_interface_name_by_index(i);  
    char *interface_type = get_interface_type_by_index(i);  
  
    if (interface_name == NULL || interface_type == NULL) {  
        // Handle NULL pointers appropriately  
        printf("Error: Interface name or type is NULL\n");  
        return _ERROR; // Return an error code  
    }  
  
    if (strcmp(interface_type, "eth") == 0)  
    {  
        // Correctly use the interface_name variable in the system call  
        char cmd[256];  
        snprintf(cmd, sizeof(cmd), "ifconfig %s down", interface_name);  
        system(cmd);  
    }  
  
    if (strcmp(interface_type, "rs485") == 0)  
    {  
        // Currently no action is taken for rs485 type, but you might want to add some  
        // For example, unexport the GPIO pin or disable the serial communication  
    }  
  
    if (strcmp(interface_type, "can") == 0)  
    {  
        // Currently no action is taken for can type, but you might want to reset the CAN  
        // controller or close the CAN channel  
    }  
  
    // If needed, free dynamically allocated memory for interface_name and interface_type  
    // (assuming they are dynamically allocated, which is not shown in the context)  
  
    return _SUCCESS; // Return success  
}  



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

    if (strcmp(interface_type, "can") == 0) {   
		// Declare a buffer for the received message, assuming it might be longer  
		DealData data;          
		data.deal_func = deal; // 将 deal 函数指针保存到结构体中
		data.linked_node = linked_node;
		data.listened_interface = source_interface;

		
		int i = get_interface_index(source_interface);
		int can_id = get_channel_id_by_index(i);
	    // Attempt to receive a packet from the source interface  
	    if (receive_packet_can(can_id,data.msg,MAX_MSG_LEN,max_waiting_time)<0) {    
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

			int index = get_interface_index(source_interface);
			int fd = get_temporary_fd(index);


			//printAllInfo();

			// Attempt to receive a packet from the source interface  
			if (receive_packet_rs485(fd,data.msg,MAX_MSG_LEN,max_waiting_time)<0) {	 
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


/*res_msg长度需要为max_message_len+1，这样字符串长度才是max_message_len*/
void fillMessageToMaxMsgLen(const char *message, char *res_msg, int max_message_len) {  
    // 首先，将原始消息复制到 RS485MSG  
    strncpy(res_msg, message, max_message_len);  
	
      
    // 如果原始消息长度大于 RS485_LEN，截断它  
    if (strlen(message) > max_message_len) {  
        res_msg[max_message_len] = '\0'; // 确保字符串正确终止  
    } else {  
        // 否则，用'\0' 填充剩余部分  
        size_t len = strlen(res_msg);  
        memset(res_msg + len, '\0', max_message_len - len);  
        // 确保字符串在末尾正确终止  
        res_msg[max_message_len] = '\0';  
    }  
      
    // 可选：打印结果以验证  
    printf("Filled message: '%s'\n", res_msg);  
}  


int send_message(const char *source_interface,const char *message)
{   

	char *interface_status = get_interface_status(source_interface);
	if(strcmp(interface_status, "receiving") == 0 || strcmp(interface_status, "closed") == 0 )
	{
		return _ERROR; /*如果是单纯receiving或closed，那么不做任何发送*/
	}

	char *interface_type = get_interface_type(source_interface);

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

		// 填充至MAX_MSG_LEN，以保证发送长度一定是MAX_MSG_LEN个字节
		char RS485MSG[MAX_MSG_LEN + 1];
		fillMessageToMaxMsgLen(message,RS485MSG,MAX_MSG_LEN);

		
		int index = get_interface_index(source_interface);
		int fd = get_temporary_fd(index);

        if (send_packet_rs485(fd, RS485MSG, MAX_MSG_LEN) < 0) {    
        	printf("send failed!\n");
        	return _ERROR; // Retry sending    
        }    
		return _SUCCESS;
    }  


    // Check if the interface type is "can" (could be removed if not needed)    
    if (strcmp(interface_type, "can") == 0) {    
		int i = get_interface_index(source_interface);
		int can_id = get_channel_id_by_index(i);

		// 填充至MAX_MSG_LEN，以保证发送长度一定是MAX_MSG_LEN个字节
		char CANMSG[MAX_MSG_LEN + 1];
		fillMessageToMaxMsgLen(message,CANMSG,MAX_MSG_LEN);
		
        if (send_packet_can(can_id,CANMSG,MAX_MSG_LEN)< 0) {    
        	printf("send failed!\n");
        	return _ERROR; // Retry sending    
        }    
		return _SUCCESS;
    }  	
  
    // This point should not be reached due to the infinite loop, but for completeness    
    return _ERROR; // In case of unexpected termination    
}





int set_status(const char *source_interface, const char *status)  
{  
	char *current_status = get_interface_status(source_interface);
	if (strcmp(current_status, status) == 0) {
		return _SUCCESS;
	}
 
  
    // Get the interface type  
    char *interface_type = get_interface_type(source_interface);  
	int i = get_interface_index(source_interface);
  
    // Check if the interface is of type 'rs485'  
    if (strcmp(interface_type, "rs485") == 0) {  
        // Compare the status string correctly using strcmp  
        if (strcmp(status, "sending_and_receiving") == 0) {  
            printf("rs485 cannot be set as 'sending_and_receiving'\n");  
            return _ERROR;  
        }  
		
  		// Attempt to set the interface status  
	    if (set_interface_status(source_interface, status) < 0) {  
	        // Print an error message (assuming a function or macro for it)  
	        printf("Failed to set interface status\n");  
	        return _ERROR;  
	    } 
		
        if (strcmp(status, "sending") == 0) {  
			// int rs485_gpio_number = get_rs485_gpio_number_by_index(i);  
        	// exportGPIO(rs485_gpio_number); // Assuming exportGPIO is defined elsewhere  
            // set485TX();  
        }  
  
        if (strcmp(status, "receiving") == 0) {  
			// int rs485_gpio_number = get_rs485_gpio_number_by_index(i);  
        	// exportGPIO(rs485_gpio_number); // Assuming exportGPIO is defined elsewhere  
            // set485RX();   
        }  
  
        if (strcmp(status, "closed") == 0) {  
            // For 'closed' status, assuming no special action is needed  
            // Just return success  
        }  

    }  


    if (strcmp(interface_type, "eth") == 0) {  
        // Compare the status string correctly using strcmp  
        if (strcmp(status, "sending") == 0 || strcmp(status, "receiving") == 0) {  
            printf("eth cannot be set as 'sending' or 'receiving'\n");  
            return _ERROR;  
        }  
		
  		// Attempt to set the interface status  
	    if (set_interface_status(source_interface, status) < 0) {  
	        // Print an error message (assuming a function or macro for it)  
	        printf("Failed to set interface status\n");  
	        return _ERROR;  
	    } 
		
        if (strcmp(status, "sending_and_receiving") == 0) {  
            // Use snprintf or similar to safely format strings for system call  
	        char cmd_up[256];  
	        snprintf(cmd_up, sizeof(cmd_up), "ifconfig %s up", source_interface);  
	        system(cmd_up);  
        }  
  
        if (strcmp(status, "closed") == 0) {  
	        char cmd_up[256];  
	        snprintf(cmd_up, sizeof(cmd_up), "ifconfig %s down", source_interface);  
	        system(cmd_up);
        }  
		

    }  

      if (strcmp(interface_type, "can") == 0) {  
        // Compare the status string correctly using strcmp  
        if (strcmp(status, "sending_and_receiving") == 0) {  
            printf("can cannot be set as 'sending_and_receiving'\n");  
            return _ERROR;  
        }  
		
  		// Attempt to set the interface status  
	    if (set_interface_status(source_interface, status) < 0) {  
	        // Print an error message (assuming a function or macro for it)  
	        printf("Failed to set interface status\n");  
	        return _ERROR;  
	    } 
		
        if (strcmp(status, "sending") == 0 || strcmp(status, "receiving") == 0) {  
        }  

 
        if (strcmp(status, "closed") == 0) {  
            // For 'closed' status, assuming no special action is needed  
            // Just return success  
        }  

    }  
    // If we reach here, either the interface type is not 'rs485' or the status is unrecognized  
    // but we're returning success as per the original function logic  
    return _SUCCESS;  
}




void sync_status(const char *source_interface)
{
	//status = sending receiving sending_and_receiving closed
	//这个先不写
	//这个不能单纯只是返回接口状态数据库中的status字段，而应该真正调用底层接口去检查接口的状态，并且把检查结果同步到数据库
	//这个在start_info之后，载入数组后要启动一下
}




