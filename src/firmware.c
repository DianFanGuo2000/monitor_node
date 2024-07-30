
#include "firmware.h"



int init_basic_interface(int i)  
{  
    char *interface_name = get_interface_name_by_index(i);  
    char *interface_type = get_interface_type_by_index(i);  
  
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
  
        // Use snprintf or similar to safely format strings for system call  
        char cmd_up[256];  
        snprintf(cmd_up, sizeof(cmd_up), "ifconfig %s up", interface_name);  
        system(cmd_up);  
  
        char cmd_addr[256];  
        snprintf(cmd_addr, sizeof(cmd_addr), "ifconfig %s %s netmask %s", interface_name, ip_addr, mask);  
        system(cmd_addr);  
    }  
  
    if (strcmp(interface_type, "rs485") == 0)  
    {  
        int rs485_gpio_number = get_rs485_gpio_number_by_index(i);  
        exportGPIO(rs485_gpio_number); // Assuming exportGPIO is defined elsewhere  
    }  

    if (strcmp(interface_type, "can") == 0)  
    {  
        int channel_id = get_channel_id_by_index(i);  
		if(comCanSTDCfgInit(channel_id)<0)
		{
			return _ERROR;
		}
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



// �̺߳����������첽������Ϣ  
void* deal_async(void* arg) {  
    DealData* data = (DealData*)arg;  
    data->deal_func(data->linked_node,data->listened_interface,data->msg); // ���� deal ����������Ϣ  
}  


int receive_message(const char *linked_node,const char *source_interface,Dealer deal,long max_waiting_time)    
{

	char *interface_status = get_interface_status(source_interface);
	if(strcmp(interface_status, "sending") == 0 || strcmp(interface_status, "closed") == 0 )
	{
		return _ERROR; /*����ǵ���sending��closed����ô�����κν���*/
	}


	char *interface_type = get_interface_type(source_interface);
	time_t begin_time = time(NULL); // Initialize start time  

    // Check if the interface type is "eth"  
    if (strcmp(interface_type, "eth") == 0) {    
	    // Declare a buffer for the received message, assuming it might be longer  
		DealData data;          
		data.deal_func = deal; // �� deal ����ָ�뱣�浽�ṹ����
		data.linked_node = linked_node;
		data.listened_interface = source_interface;
		
	    // Attempt to receive a packet from the source interface  
	    if (receive_packet(source_interface, data.msg, max_waiting_time)<0) {    
	        // If receiving the message fails, return an error and don't reply
	        return _ERROR;    
	    }    


        pthread_t thread_id;  
        if (pthread_create(&thread_id, NULL, deal_async, &data) != 0) {  
            // �̴߳���ʧ�ܴ������ش���  
            return _ERROR;  
        }  

	    return _SUCCESS;    
    }


	
	if (strcmp(interface_type, "rs485") == 0) {	 
			// Declare a buffer for the received message, assuming it might be longer  
			DealData data;			
			data.deal_func = deal; // �� deal ����ָ�뱣�浽�ṹ����
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
				// �̴߳���ʧ�ܴ������ش���  
				return _ERROR;	
			}  

			close_port(fd);
	
			return _SUCCESS;	
	}	 

}


void fillMessageToRS485Len(const char *message, char *RS485MSG, int rs485_len) {  
    // ���ȣ���ԭʼ��Ϣ���Ƶ� RS485MSG  
    strncpy(RS485MSG, message, rs485_len);  
	
      
    // ���ԭʼ��Ϣ���ȴ��� RS485_LEN���ض���  
    if (strlen(message) > rs485_len) {  
        RS485MSG[rs485_len] = '\0'; // ȷ���ַ�����ȷ��ֹ  
    } else {  
        // ������'\0' ���ʣ�ಿ��  
        size_t len = strlen(RS485MSG);  
        memset(RS485MSG + len, '\0', rs485_len - len);  
        // ȷ���ַ�����ĩβ��ȷ��ֹ  
        RS485MSG[rs485_len] = '\0';  
    }  
      
    // ��ѡ����ӡ�������֤  
    printf("Filled message: '%s'\n", RS485MSG);  
}  


int send_message(const char *source_interface,const char *message)
{   

	char *interface_status = get_interface_status(source_interface);
	if(strcmp(interface_status, "receiving") == 0 || strcmp(interface_status, "closed") == 0 )
	{
		return _ERROR; /*����ǵ���receiving��closed����ô�����κη���*/
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

		// �����RS485_LEN���Ա�֤���ͳ���һ����RS485_LEN���ֽ�
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





int set_status(const char *source_interface, const char *status)  
{  
    // Attempt to set the interface status  
    if (set_interface_status(source_interface, status) < 0) {  
        // Print an error message (assuming a function or macro for it)  
        printf("Failed to set interface status\n");  
        return _ERROR;  
    }  
  
    // Get the interface type  
    char *interface_type = get_interface_type(source_interface);  
  
    // Check if the interface is of type 'rs485'  
    if (strcmp(interface_type, "rs485") == 0) {  
        // Compare the status string correctly using strcmp  
        if (strcmp(status, "sending_and_receiving") == 0) {  
            printf("rs485 cannot be set as 'sending_and_receiving'\n");  
            return _ERROR;  
        }  
  
        if (strcmp(status, "sending") == 0) {  
            set485TX();  
            return _SUCCESS;  
        }  
  
        if (strcmp(status, "receiving") == 0) {  
            set485RX();  
            return _SUCCESS;  
        }  
  
        if (strcmp(status, "closed") == 0) {  
            // For 'closed' status, assuming no special action is needed  
            // Just return success  
            return _SUCCESS;  
        }  
    }  
  
    // If we reach here, either the interface type is not 'rs485' or the status is unrecognized  
    // but we're returning success as per the original function logic  
    return _SUCCESS;  
}




void sync_status(const char *source_interface)
{
	//status = sending receiving sending_and_receiving closed
	//����Ȳ�д
	//������ܵ���ֻ�Ƿ��ؽӿ�״̬���ݿ��е�status�ֶΣ���Ӧ���������õײ�ӿ�ȥ���ӿڵ�״̬�����ҰѼ����ͬ�������ݿ�
	//�����start_info֮�����������Ҫ����һ��
}




