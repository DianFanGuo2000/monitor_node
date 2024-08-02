#ifndef INFO_MANAGER_MANAGER_H  
#define INFO_MANAGER_MANAGER_H  


#include "../configures.h"


#include <time.h> // For time functions 


#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <cjson/cJSON.h> /*��Ҫ��װcJSON */
#include <stdbool.h>  





struct eth_info { 
	char *ip_addr;  
	char *net_mask;  

    char *mac_addr;  
};  
  
struct can_info {  
    int can_id;  
};  
  
struct rs485_info { 
	int rs485_gpio_number;
	
	int databits;
	int stopbits;
	unsigned char paritybits;
    int baud_rate;  

	int temporary_fd;
};

struct interface_info {
	char *located_node;
    char *interface_name;  
    char *interface_type;  
	char *linked_node;
    char *linked_interface_name;  
    char *linked_interface_type;  
    char *center_interface_name;  

    struct eth_info eth_info;
	struct eth_info linked_eth_info;
    struct can_info can_info;
	struct can_info linked_can_info;
    struct rs485_info rs485_info;
	struct rs485_info linked_rs485_info;


	char *mode;
	char *status;
};


struct communication_info{
	char *linked_node;
	char *interface_name;
	time_t updated_time;
	//char *error_ratio_value;
	unsigned long tx;
	unsigned long rx;
};


struct communication_info *communication_info_array;
int communication_info_cnt;


struct interface_info *interface_info_array;
int interface_cnt;


time_t test_begin_time; 


time_t get_test_begin_time();

int update_interface_cnt(const char *filename);
void start_and_load_info(const char *filename);
void dump_info_and_close(const char *filename);

void write_interface_info_array_to_json(const char *filename,struct interface_info *array, size_t size);
void read_interface_info_array_from_json(const char *filename, struct interface_info *array, size_t size);

int string_to_time_t(const char* time_buffer, time_t* parsed_time);
int time_t_to_string(time_t time_val, char* buffer, size_t buffer_size);

char* get_interface_type_by_index(int i);


char *get_linked_node(int i);

int get_interface_cnt();
char* get_interface_name_by_index(int i);
char* get_linked_interface_name_by_index(int i);


char* get_mac_addr(const char *interface_name);
char* get_linked_mac_addr(const char *interface_name);

char* get_interface_type(const char *interface_name);

void print_interface_info(const struct interface_info *info);
void print_interface_info_array(const struct interface_info *array, int size);
void printAllInfo();

char *get_located_node(int i);

int is_this_interface_in_current_node(const char* interface_name);

char* get_interface_name_by_linked_interface_name(char* linked_interface_name);

void malloc_communication_info_array(int size);
void free_communication_info_array();

char* parse_communication_info_array_to_json();

int update_communication_info_array_from_json(char* communication_info_array_json_str);


int update_communication_info_array(char* linked_node,char* interface_name,time_t updated_time,unsigned long tx,unsigned long rx); //,char* error_ratio_value
void string_to_unsigned_long(const char* str, unsigned long* result);

int get_interface_index(const char* interface_name);


char* get_interface_status(const char* interface_name);
char* get_interface_mode(const char* interface_name);



int  get_baud_rate_by_index(int i);


int  get_databits_by_index(int i);


unsigned char  get_paritybits_by_index(int i);


int  get_stopbits_by_index(int i);


// Function to check if the status is valid  
bool isValidStatus(const char* status);
  
int set_interface_status(const char* interface_name, const char* status);

char* get_ip_addr_by_index(int i);	
char* get_net_mask_by_index(int i);	
int get_rs485_gpio_number_by_index(int i);	
int get_channel_id_by_index(int i); 


int  get_temporary_fd(int i);

void  set_temporary_fd(int i,int fd);

char* get_center_interface_name(int i);

void print_communication_info(const struct communication_info*info);



void print_communication_info_array(const struct communication_info *array, int size);

void printAllCommucationInfo();


#endif
