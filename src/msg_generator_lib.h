#ifndef MSG_GENERATOR_LIB_H  
#define MSG_GENERATOR_LIB_H  
#include <stdio.h> 

#include "configures.h"
#include "base/info_manager.h"


char *msg_generator_name_array[] = {"generate_eth_test_msg", "generate_rs485_test_msg", "generate_fpu_can_test_msg", "generate_gpu_can_test_msg"};  
int msg_generator_num = 4;

int msg_generator_transfer(char *msg_generator_name, const char *interface_name, const char *mode, const int current_round,char *msg);

int generate_eth_test_msg(const char *interface_name, const char *mode, const int current_round,char *msg);

int generate_rs485_test_msg(const char *interface_name, const char *mode, const int current_round,char *msg);

int generate_fpu_can_test_msg(const char *interface_name, const char *mode, const int current_round,char *msg);

int generate_gpu_can_test_msg(const char *interface_name, const char *mode, const int current_round,char *msg);


#endif

