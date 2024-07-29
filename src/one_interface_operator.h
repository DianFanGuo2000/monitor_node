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


// 函数指针类型定义，用于根据当前轮次选择状态索引  
typedef char* (*status_chooser)(const char *interface_name,const int current_round);  
  

  



void update_status_in_current_round(const char *updated_interface, status_chooser choose,int current_round);


void set_res_file_name(char *file_name);
void set_center_interface_name(char *interface_name);
int sync_communication_info(const char* center_interface_name);


char* generate_eth_reply(const char* msg);

void listen_upon_one_interface_in_one_time(char *linked_node, char *listened_interface,status_chooser choose);



void test_upon_one_interface_in_one_time(const char *test_interface,const char *message,int packages_num,status_chooser choose);



#endif
