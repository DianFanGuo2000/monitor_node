#ifndef ONE_INTERFACE_OPERATOR_H  
#define ONE_INTERFACE_OPERATOR_H  

#include <pthread.h>  
#include <stdio.h>  
#include <stdlib.h>  
#include "firmware.h"


typedef struct {  
    const char *test_interface;  
    const char *message;
	long max_waiting_time_in_one_sending_time;
    long max_waiting_time_in_all;  
    const char *expected_reply_str;   
} ThreadArgs;  

char *res_file_name;

void set_res_file_name(char *file_name);

char* generate_eth_reply(const char* msg);

void listen_upon_one_interface(char *listened_interface);


char *test_upon_one_interface_threaded(const char *test_interface,  
                                      const char *message, 
                                      long max_waiting_time_in_one_sending_time, 
                             		  long max_waiting_time_in_all,
                                      int packages_num,  
                                      const char *expected_reply_str);



#endif
