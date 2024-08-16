
#include "firmware.h"



int assigned_flag = 0;
pthread_mutex_t assigned_flag_lock;


// 在程序初始化时创建锁  
void initialize_assigned_flag_lock() {  
    pthread_mutex_init(&assigned_flag_lock, NULL);  
}  
  
// 在程序结束时销毁锁  
void destroy_assigned_flag_lock() {  
    pthread_mutex_destroy(&assigned_flag_lock);  
}




// 线程函数，用于异步处理消息  
void* deal_async(void* arg) {  
    DealData* data = (DealData*)arg;  
	//printf("data->msg:%s \n",data->msg);

	if(data->linked_node==NULL)
	{
		printf("[ERROR] deal_async got a NULL data->linked_node!\n");
		return ;
	}
	
	if(data->listened_interface==NULL)
	{
		printf("[ERROR] deal_async got a NULL data->listened_interface!\n");
		return ;
	}

	if(data->msg==NULL)
	{
		printf("[ERROR] deal_async got a NULL data->msg!\n");
		return ;
	}
	char TEMP_NODE[MAX_IF_LEN];
	char TEMP_INF[MAX_IF_LEN];
	char TEMP_MSG[MAX_MSG_LEN];

	strncpy(TEMP_NODE,data->linked_node , MAX_MSG_LEN); 
	strncpy(TEMP_INF, data->listened_interface, MAX_MSG_LEN); 
	strncpy(TEMP_MSG, data->msg, MAX_MSG_LEN); 

    assigned_flag = 1; // 发送信号通知父线程已结束赋值了
    data->deal_func(TEMP_NODE,TEMP_INF,TEMP_MSG); // 调用 deal 函数处理消息  
}  



// Declare a buffer for the received message, assuming it might be longer  
//DealData data;	/*千万注意，这是一个全局变量，所以一定要加锁来访问，不然可能会引发奇奇怪怪的错误*/
int receive_message(const char *linked_node,const char *source_interface,Dealer deal,long max_waiting_time)    
{
	if(linked_node==NULL)
	{
		printf("[ERROR] receive_message got a NULL linked_node!\n");
		return _ERROR;
	}
	
	if(source_interface==NULL)
	{
		printf("[ERROR] receive_message got a NULL source_interface!\n");
		return _ERROR;
	}

	int index = get_interface_index(source_interface);
	char *interface_status = get_interface_status_by_index(index);
	if(strcmp(interface_status, "sending") == 0 || strcmp(interface_status, "closed") == 0 )
	{
		return _ERROR; /*如果是单纯sending或closed，那么不做任何接收*/
	}

	
	char *base_receive_func = get_base_receive_func_by_index(index);
	time_t begin_time = time(NULL); // Initialize start time  

    // Check if the interface type is "eth"  
    if (strcmp(base_receive_func, "receive_packet") == 0) {   
		DealData data;
		data.deal_func = deal; // 将 deal 函数指针保存到结构体中
		data.linked_node = linked_node;
		data.listened_interface = source_interface;
		
	    // Attempt to receive a packet from the source interface  
		char TEMP_MSG[MAX_MSG_LEN]={0};
		if (receive_packet(get_ip_name_by_index(index),TEMP_MSG,max_waiting_time)<0) {  
			//usleep(3000000);
			printf("failed to got message from \"%s\"!\n",source_interface);
			printf("TEMP_MSG:%s\n",TEMP_MSG);
			return _ERROR;	  
		}	 
		strncpy(data.msg, TEMP_MSG, MAX_MSG_LEN); // 不直接拿data.msg作为形参，防止其随着原函数声明周期结束而被析构


		pthread_mutex_lock(&assigned_flag_lock);  
        pthread_t thread_id;  
        if (pthread_create(&thread_id, NULL, deal_async, &data) != 0) {  
            // 线程创建失败处理，返回错误  
            return _ERROR;  
        }  
		
		while(!assigned_flag);
		assigned_flag = 0;	
		pthread_mutex_unlock(&assigned_flag_lock);	

	    return _SUCCESS;    
    }

    if (strcmp(base_receive_func, "receive_packet_can_fpu") == 0) { 
		DealData data;
		data.deal_func = deal; // 将 deal 函数指针保存到结构体中
		data.linked_node = linked_node;
		data.listened_interface = source_interface;

		
		int can_id = get_channel_id_by_index(index);
	    // Attempt to receive a packet from the source interface    
		char TEMP_MSG[MAX_CAN_DATA_LENGTH];
		if (receive_packet_can_fpu(can_id,TEMP_MSG,MAX_CAN_DATA_LENGTH,max_waiting_time)<0) {  
			printf("failed to got message from \"%s\"!\n",source_interface);
			printf("TEMP_MSG:%s\n",TEMP_MSG);
			return _ERROR;	  
		}	 
		strncpy(data.msg, TEMP_MSG, MAX_CAN_DATA_LENGTH); // 不直接拿data.msg作为形参，防止其随着原函数声明周期结束而被析构

		pthread_mutex_lock(&assigned_flag_lock);  
        pthread_t thread_id;  
        if (pthread_create(&thread_id, NULL, deal_async, &data) != 0) {  
            // 线程创建失败处理，返回错误  
            return _ERROR;  
        }  

		
		while(!assigned_flag);
		assigned_flag = 0;	
		pthread_mutex_unlock(&assigned_flag_lock);	


	    return _SUCCESS;    
    }


	if (strcmp(base_receive_func, "receive_packet_can_gpu") == 0) {
		DealData data;
		data.deal_func = deal; // 将 deal 函数指针保存到结构体中
		data.linked_node = linked_node;
		data.listened_interface = source_interface;
    //printf("date_node: %s, data_listened:%s\n", data.linked_node, data.listened_interface);
		
		int can_id = get_channel_id_by_index(index);
	    // Attempt to receive a packet from the source interface    
		char TEMP_MSG[MAX_CAN_DATA_LENGTH];
		if (receive_packet_can_gpu(can_id,TEMP_MSG,MAX_CAN_DATA_LENGTH,max_waiting_time)<0) {  
			printf("failed to got message from \"%s\"!\n",source_interface);
			printf("TEMP_MSG:%s\n",TEMP_MSG);
			return _ERROR;	  
		}	 
		strncpy(data.msg, TEMP_MSG, MAX_CAN_DATA_LENGTH); // 不直接拿data.msg作为形参，防止其随着原函数声明周期结束而被析构

		pthread_mutex_lock(&assigned_flag_lock);  
        pthread_t thread_id;  
        if (pthread_create(&thread_id, NULL, deal_async, &data) != 0) {  
            // 线程创建失败处理，返回错误  
            return _ERROR;  
        }  

		
		while(!assigned_flag);
		assigned_flag = 0;	
		pthread_mutex_unlock(&assigned_flag_lock);	


	    return _SUCCESS;    
    }


	
	if (strcmp(base_receive_func, "receive_packet_rs485") == 0) {	 
			DealData data;
			data.deal_func = deal; // 将 deal 函数指针保存到结构体中
			data.linked_node = linked_node;
			data.listened_interface = source_interface;

			int fd = get_temporary_fd(index);
			//printAllInfo();

			char TEMP_MSG[MAX_MSG_LEN];
			// Attempt to receive a packet from the source interface  
			if (receive_packet_rs485(fd,TEMP_MSG,MAX_MSG_LEN,max_waiting_time)<0) {	 
				printf("failed to got message from \"%s\"!\n",source_interface);
				printf("TEMP_MSG:%s\n",TEMP_MSG);
				return _ERROR;	  
			}	 
			strncpy(data.msg, TEMP_MSG, MAX_MSG_LEN); // 不直接拿data.msg作为形参，防止其随着原函数声明周期结束而被析构

			
			//printAllInfo();

			//printf("%s\n",data.msg);
			pthread_mutex_lock(&assigned_flag_lock);  
			pthread_t thread_id;  
			if (pthread_create(&thread_id, NULL, deal_async, &data) != 0) {  
				// 线程创建失败处理，返回错误  
				return _ERROR;	
			}  
			
			while(!assigned_flag);
			assigned_flag = 0;  
			pthread_mutex_unlock(&assigned_flag_lock);  

			return _SUCCESS;	
	}	 

}


/*res_msg长度需要为max_message_len+1，这样字符串长度才是max_message_len*/
void fillMessageToMaxMsgLen(const char *message, char *res_msg, int max_message_len) {  


	if(res_msg==NULL)
	{
		printf("[ERROR] fillMessageToMaxMsgLen got a NULL res_msg!\n");
		return _ERROR;
	}
	
	if(message==NULL)
	{
		printf("[ERROR] fillMessageToMaxMsgLen got a NULL message!\n");
		return _ERROR;
	}

	


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
    //intf("Filled message: '%s'\n", res_msg);  
}  


int send_message(const char *source_interface,const char *message)
{   

	if(source_interface==NULL)
	{
		printf("[ERROR] send_message got a NULL source_interface!\n");
		return _ERROR;
	}
	
	if(message==NULL)
	{
		printf("[ERROR] send_message got a NULL message!\n");
		return _ERROR;
	}


	int index = get_interface_index(source_interface);
	char *interface_status = get_interface_status_by_index(index);
	if(strcmp(interface_status, "receiving") == 0 || strcmp(interface_status, "closed") == 0 )
	{
		return _ERROR; /*如果是单纯receiving或closed，那么不做任何发送*/
	}

	char *base_send_func = get_base_send_func_by_index(index);
    // Check if the interface type is "eth" (could be removed if not needed)    
    if (strcmp(base_send_func, "send_packet") == 0) {    
        // Retrieve the MAC addresses    
        const char *src_mac = get_mac_addr(source_interface);    
        const char *dest_mac = get_linked_mac_addr(source_interface);  
        // Check if MAC addresses were retrieved and packet can be sent    
        if (send_packet(get_ip_name_by_index(index), message, src_mac, dest_mac) < 0) {    
        	printf("send failed!\n");
        	return _ERROR; // Retry sending    
        }    
		return _SUCCESS;
    }  


    // Check if the interface type is "rs485" (could be removed if not needed)    
    if (strcmp(base_send_func, "send_packet_rs485") == 0) {  

		// 填充至MAX_MSG_LEN，以保证发送长度一定是MAX_MSG_LEN个字节
		char RS485MSG[MAX_MSG_LEN];
		fillMessageToMaxMsgLen(message,RS485MSG,MAX_MSG_LEN-1);// 确保最后一位RS485MSG[MAX_MSG_LEN-1]一定是'\0'
		int fd = get_temporary_fd(index);

        if (send_packet_rs485(fd, RS485MSG, MAX_MSG_LEN) < 0) {    
        	printf("send failed!\n");
        	return _ERROR; // Retry sending    
        }    
		return _SUCCESS;
    }  


    // Check if the interface type is "can" (could be removed if not needed)    
    if (strcmp(base_send_func, "send_packet_can_fpu") == 0) {    
		int can_id = get_channel_id_by_index(index);

		// 填充至MAX_MSG_LEN，以保证发送长度一定是MAX_MSG_LEN个字节
		char CANMSG[MAX_CAN_DATA_LENGTH];
		fillMessageToMaxMsgLen(message,CANMSG,MAX_CAN_DATA_LENGTH-1);// 确保最后一位CANMSG[MAX_CAN_DATA_LENGTH -1]一定是'\0'
		
        if (send_packet_can_fpu(can_id,CANMSG,MAX_CAN_DATA_LENGTH)< 0) {    
        	printf("send failed!\n");
        	return _ERROR; // Retry sending    
        }    
		return _SUCCESS;
    }  	


	if (strcmp(base_send_func, "send_packet_can_gpu") == 0) {    
		int can_id = get_channel_id_by_index(index);

		// 填充至MAX_MSG_LEN，以保证发送长度一定是MAX_MSG_LEN个字节
		char CANMSG[MAX_CAN_DATA_LENGTH];
		fillMessageToMaxMsgLen(message,CANMSG,MAX_CAN_DATA_LENGTH-1);// 确保最后一位CANMSG[MAX_CAN_DATA_LENGTH -1]一定是'\0'
		
        if (send_packet_can_gpu(can_id,CANMSG,MAX_CAN_DATA_LENGTH)< 0) {    
        	printf("send failed!\n");
        	return _ERROR; // Retry sending    
        }    
		return _SUCCESS;
    }  	


    // This point should not be reached due to the infinite loop, but for completeness    
    return _ERROR; // In case of unexpected termination    
}



// 函数用于检查接口状态  
int is_interface_up(const char* interface) {  
    char cmd[256];  
    snprintf(cmd, sizeof(cmd), "ifconfig %s | grep -q 'inet'", interface);  
    return system(cmd) == 0;  
}  


int set_status(const char *source_interface, const char *status)  
{  

	if(source_interface==NULL)
	{
		printf("[ERROR] set_status got a NULL source_interface!\n");
		return _ERROR;
	}
	
	if(status==NULL)
	{
		printf("[ERROR] set_status got a NULL status!\n");
		return _ERROR;
	}


	char *current_status = get_interface_status(source_interface);
	if (strcmp(current_status, status) == 0) {
		return _SUCCESS;
	}
 
  
    // Get the interface type  
    //char *interface_type = get_interface_type(source_interface);  
	int i = get_interface_index(source_interface);
  	char *base_send_func = get_base_send_func_by_index(i);
	char *base_receive_func = get_base_receive_func_by_index(i);

	
    // Check if the interface is of type 'rs485'  
    if (strcmp(base_send_func, "send_packet_rs485") == 0 && strcmp(base_receive_func, "receive_packet_rs485") == 0) {  
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


    if (strcmp(base_send_func, "send_packet") == 0 && strcmp(base_receive_func, "receive_packet") == 0) {  
        // Compare the status string correctly using strcmp  
       /* if (strcmp(status, "sending") == 0 || strcmp(status, "receiving") == 0) {  
            printf("eth cannot be set as 'sending' or 'receiving'\n");  
            return _ERROR;  
        }  */
		
  		// Attempt to set the interface status  
	    if (set_interface_status(source_interface, status) < 0) {  
	        // Print an error message (assuming a function or macro for it)  
	        printf("Failed to set interface status\n");  
	        return _ERROR;  
	    } 
		
        if (strcmp(status, "sending_and_receiving") == 0 && !is_interface_up(source_interface)) {  
            char cmd_up[256];  
            snprintf(cmd_up, sizeof(cmd_up), "ifconfig %s up", source_interface);  
            system(cmd_up);  
        }  

  
        if (strcmp(status, "closed") == 0 && is_interface_up(source_interface)) {  
	        char cmd_up[256];  
	        snprintf(cmd_up, sizeof(cmd_up), "ifconfig %s down", source_interface);  
	        system(cmd_up);
        }  
		

    }  

      if (strcmp(base_send_func, "send_packet_can_fpu") == 0 && strcmp(base_receive_func, "receive_packet_can_fpu") == 0) {  
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


	if (strcmp(base_send_func, "send_packet_can_gpu") == 0 && strcmp(base_receive_func, "receive_packet_can_gpu") == 0) {  
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




