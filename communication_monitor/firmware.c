
#include "firmware.h"



// Implementation of the receive_and_reply function  
/*这里存在一个时序问题需要解决，即发送方发送完msg后，到发送方正式开始等待回复之前，接收方就已经回复了，导致发送方无法收到回复*/
int receive_and_reply(const char *source_interface,        
                      ReplyGenerator generate_reply,
                      long max_waiting_time)    
{    
	char *interface_type = get_interface_type(source_interface);
	time_t begin_time = time(NULL); // Initialize start time  
    // Check if the interface type is "eth"  
    
    if (strcmp(interface_type, "eth") == 0) {    
	    // Declare a buffer for the received message, assuming it might be longer  
	    char msg[MAX_MSG_LEN];   
	    // Attempt to receive a packet from the source interface  
	    if (receive_packet(source_interface, msg, max_waiting_time)<0) {    
	        // If receiving the message fails, return an error and don't reply
	        return _ERROR;    
	    }    

		
	    // Generate a reply based on the received message  
	    char *reply = generate_reply(msg);    
	    // Check if the reply generation failed  
	    if (reply == NULL) {    
	        // If the reply is NULL, indicating a failure to generate the reply, return an error and don't reply
	        return _ERROR;    
	    }    
		

		
	  	struct timespec delay;
		delay.tv_sec = 0;  // 秒      
		delay.tv_nsec = 63000000;
	    nanosleep(&delay, NULL); // 等一会，让对方可以有时间来准备接
  
		
	    // Retrieve the MAC addresses of the source and destination interfaces  
	    const char *src_mac = get_mac_addr(source_interface);    
	    const char *dest_mac = get_linked_mac_addr(source_interface); 
		//printf("%s\n",src_mac);
		//printf("%s\n",dest_mac);
	    // Check if the MAC addresses were successfully retrieved and the packet can be sent  
	    if (send_packet(source_interface, reply, src_mac, dest_mac)<0) {    
	        // If either MAC address is missing or the packet fails to send, free the reply and return an error  
	        printf("reply failed!\n");
	        //free(reply); // 注意，如果是自己分配了内存的char指针，请在函数外free，为了防止free非动态分配的内存区域引起段错误，此处不free
	        return _ERROR;    
	    }    
		//printf("xass\n");

	    
	    // Free the memory allocated for the reply string  
	    //free(reply);    // 注意，如果是自己分配了内存的char指针，请在函数外free，为了防止free非动态分配的内存区域引起段错误，此处不free
	    // Return success  
	    return _SUCCESS;    
    }    

}



// Implementation of send_until_being_replied  
int send_until_being_replied(const char *source_interface,     
                             const char *message,   
                             long max_waiting_time_in_one_sending_time, 
                             long max_waiting_time_in_all,     
                             int max_send_times,     
                             const char* expected_reply_str)    
{    
	char *interface_type = get_interface_type(source_interface);
	char msg[MAX_MSG_LEN]; // Buffer for the received message    
    int cnt = 0; // Initialize counter    
    time_t begin_time = time(NULL); // Initialize start time  
  
    // Check if the interface type is "eth" (could be removed if not needed)    
    if (strcmp(interface_type, "eth") == 0) {    
        while (1) {    
            if (cnt >= max_send_times) {    
                // Reached maximum send attempts    
                printf("Times reach the max!\n");
                return _ERROR;    
            }    
  
            // Retrieve the MAC addresses    
            const char *src_mac = get_mac_addr(source_interface);    
            const char *dest_mac = get_linked_mac_addr(source_interface);  
            // Check if MAC addresses were retrieved and packet can be sent    
            if (send_packet(source_interface, message, src_mac, dest_mac) < 0) {    
                cnt++;    
				printf("send failed!\n");
                continue; // Retry sending    
            }    
  
            // Attempt to receive a packet with a timeout    
            time_t current_time = time(NULL);  
            long elapsed_time = difftime(current_time, begin_time);  
            if (elapsed_time >= max_waiting_time_in_all) {  
				printf("Timeout occurred!\n");
                return _ERROR; // Timeout occurred  
            }  
  
            if (receive_packet(source_interface, msg, max_waiting_time_in_one_sending_time) < 0) {    
                cnt++;    
				printf("cannot get reply!\n");
                continue; // Timeout or receive error, retry sending    
            }    
  
            // Compare the received message with the expected reply    
            if (strcmp(msg, expected_reply_str) == 0) {    
                // Received the expected reply    
                return _SAME;    
            }else{
				return _NOT_SAME;   
				
            }

            // Received a different reply or no specific reply identifier, continue sending    
            cnt++;    
  
            // Reset the start time for each new send attempt (optional, depending on requirements)  
            // begin_time = time(NULL); // Uncomment if needed  
        }    
    }  
  
    // This point should not be reached due to the infinite loop, but for completeness    
    return _ERROR; // In case of unexpected termination    
}  



