#ifndef TEST_RESULT_MANAGER_H  
#define TEST_RESULT_MANAGER_H  

#include "configures.h"
#include <time.h> // For time functions 


#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <cjson/cJSON.h> /*需要安装cJSON */



struct eth_info {  
    char *mac_addr;  
};  
  
struct can_info {  
    int can_id;  
};  
  
struct rs485_info {  
    int baud_rate;  
};

struct interface_info {
	char *located_node;
    char *interface_name;  
    char *interface_type;  
	char *linked_node;
    char *linked_interface_name;  
    char *linked_interface_type;  
	char *center_node;
    char *center_interface_name;  
    char *center_interface_type;  

    struct eth_info eth_info;
	struct eth_info linked_eth_info;
    struct can_info can_info;
	struct can_info linked_can_info;
    struct rs485_info rs485_info;
	struct rs485_info linked_rs485_info;

	char *status;
};


struct communication_info{
	char *interface_name;
	time_t updated_time;
	char *error_ratio_value;
};


struct communication_info *communication_info_array;
int communication_info_cnt;


struct interface_info *interface_info_array;
int interface_cnt;

int update_interface_cnt(const char *filename);
void start_and_load_info(const char *filename);
void dump_info_and_close(const char *filename);

void write_interface_info_array_to_json(const char *filename,struct interface_info *array, size_t size);
void read_interface_info_array_from_json(const char *filename, struct interface_info *array, size_t size);


int get_interface_cnt();
char* get_interface_name(int i);
char* get_linked_interface_name(int i);


char* get_mac_addr(const char *interface_name);
char* get_linked_mac_addr(const char *interface_name);

char* get_interface_type(const char *interface_name);

void print_interface_info(const struct interface_info *info);
void print_interface_info_array(const struct interface_info *array, int size);
void printAllInfo();



int is_this_interface_in_current_node(const char* interface_name);

char* get_interface_name_by_linked_interface_name(char* linked_interface_name);

void malloc_communication_info_array(int size);
void free_communication_info_array();

char* parse_communication_info_array_to_json();

int update_communication_info_array_from_json(char* communication_info_array_json_str);


int update_communication_info_array(char* interface_name,time_t updated_time,char* error_ratio_value);

		

#endif
