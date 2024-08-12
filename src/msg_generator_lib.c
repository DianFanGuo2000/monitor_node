
#include "msg_generator_lib.h"


int msg_generator_transfer(char *msg_generator_name, const char *interface_name, const char *mode, const int current_round,char *msg)
{
    int i;  
    int found = 0;  
    for (i = 0; i < msg_generator_num; i++) {  
        if (strcmp(msg_generator_name, msg_generator_name_array[i]) == 0) {  
            found = 1;  
            break;  
        }  
    }  
    if (!found) {  
        return _ERROR;  
    }  
  
    if (strcmp(msg_generator_name, "generate_eth_test_msg") == 0) {  
        return generate_eth_test_msg(interface_name, mode, current_round,msg);  
    }  
    if (strcmp(msg_generator_name, "generate_rs485_test_msg") == 0) {  
        return generate_rs485_test_msg(interface_name, mode, current_round,msg);  
    }  
    if (strcmp(msg_generator_name, "generate_fpu_can_test_msg") == 0) {  
        return generate_fpu_can_test_msg(interface_name, mode, current_round,msg);  
    }  
    if (strcmp(msg_generator_name, "generate_gpu_can_test_msg") == 0) {  
        return generate_gpu_can_test_msg(interface_name, mode, current_round,msg);  
    }  

	return _ERROR;  

}

  
int generate_eth_test_msg(const char *interface_name, const char *mode, const int current_round, char *msg) {    
    sprintf(msg, "round %d test for %s", current_round, interface_name);  
	return _SUCCESS;
}  
  
int generate_rs485_test_msg(const char *interface_name, const char *mode, const int current_round, char *msg) {    
    sprintf(msg, "round %d test for %s", current_round, interface_name); 
	return _SUCCESS;
}  
  
int generate_fpu_can_test_msg(const char *interface_name, const char *mode, const int current_round, char *msg) {    
	int index = get_interface_index(interface_name);
    sprintf(msg, "can%dtest", get_channel_id_by_index(index));
	return _SUCCESS;
}  
  
int generate_gpu_can_test_msg(const char *interface_name, const char *mode, const int current_round, char *msg) {    
	int index = get_interface_index(interface_name);
    sprintf(msg, "can%dtest", get_channel_id_by_index(index));  
	return _SUCCESS;
}

