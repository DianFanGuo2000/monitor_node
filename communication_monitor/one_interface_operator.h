#ifndef ONE_INTERFACE_OPERATOR_H  
#define ONE_INTERFACE_OPERATOR_H  

#include <pthread.h>  
#include <stdio.h>  
#include <stdlib.h>  
#include "firmware.h"


typedef struct {  
    const char *test_interface;  
    const char *message;
} ThreadArgs;  

char *res_file_name;
char* center_interface_name;



void set_res_file_name(char *file_name);
void set_center_interface_name(char *interface_name);
int sync_communication_info(const char* center_interface_name);


char* generate_eth_reply(const char* msg);

void listen_upon_one_interface_in_one_time(char *linked_node, char *listened_interface);



void test_upon_one_interface_in_one_time(const char *test_interface,const char *message,int packages_num);



#endif
