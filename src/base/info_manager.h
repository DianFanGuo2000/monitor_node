#ifndef INFO_MANAGER_MANAGER_H  
#define INFO_MANAGER_MANAGER_H  


#include "../configures.h"


#include <time.h> // For time functions 


#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <cjson/cJSON.h> /*锟斤拷要锟斤拷装cJSON */
#include <stdbool.h>  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <linux/if_packet.h>




struct eth_info { 
	char* ip_name;
	char *ip_addr;  
	char *net_mask;  

    char *mac_addr;  

	int temporary_sockfd;
	struct sockaddr_ll *sock_addr; 
};  
  
struct can_info {  
    int can_id;  
	int baud_rate; 
};  
  
struct rs485_info { 
	char* rs485_dev_path;
	int rs485_gpio_number;
	
	int databits;
	int stopbits;
	unsigned char paritybits;
    int baud_rate;  

	int temporary_fd;
};

struct interface_info {
	char *mode;
	char *located_node;
    char *interface_name;  
    char *interface_type;  
	char *linked_node;
    char *linked_interface_name;  
    char *linked_interface_type;  
    char *center_interface_name;  
	char *status_chooser;  
	char *base_send_func;  
	char *base_receive_func;  
	char *msg_generator_of_sender;
	char *initializer_name;
	char *closer_name;

    struct eth_info eth_info;
	struct eth_info linked_eth_info;
    struct can_info can_info;
	struct can_info linked_can_info;
    struct rs485_info rs485_info;
	struct rs485_info linked_rs485_info;

	char *status;
	char *duplex;
	int initialized_flag; //表示还未初始化
	time_t last_work_time;
};


struct communication_info{
	char *linked_node;
	char *interface_name;
	time_t updated_time;
	//char *error_ratio_value;
	unsigned long tx;
	unsigned long rx;
	int current_round;

	int if_newest_flag;// 是否写入到结果文件中了，写了代表不是最新的，为-1，没写代表是最新的，为1
};


struct communication_info *communication_info_array;
int communication_info_cnt;


struct interface_info *interface_info_array;
int interface_cnt;


time_t test_begin_time; 


void initialize_communication_info_lock();

void destroy_communication_info_lock();

int get_sync_flag(int i);

int set_sync_flag(int i,int flag);

int get_initialized_flag_by_index(int i);
void set_initialized_flag_by_index(int i,int flag);

int get_temporary_sockfd_by_index(int i);
void set_temporary_sockfd_by_index(int i,int sockfd);

struct sockaddr_ll* get_sock_addr_value_addr(int i);
void set_sock_addr_value_addr(int i,struct sockaddr_ll* addr);



char* parse_communication_info_array_with_certain_listen_interface_to_json(const char *certain_listen_interface);

void printNewestCommucationInfo();
char* parse_newest_communication_infos_to_json(int max_info_num_one_time, int whether_used_for_sync);

time_t get_test_begin_time();
char* get_interface_status_by_index(int i);

int update_interface_cnt_from_split_json(const char *filename);
int update_interface_cnt_from_overall_json(const char *current_node_name, const char *filename);


void start_info_manager_from_split_json(const char *filename);
void start_info_manager_from_overall_json(const char* current_node_name, const char *filename);

void close_info_manager(const char *filename);

void write_interface_info_array_to_json(const char *filename,struct interface_info *array, size_t size);
void read_interface_info_array_from_split_json(const char *filename, struct interface_info *array);
void  read_interface_info_array_from_overall_json(const char *current_node_name, const char *filename, struct interface_info *array);

int get_all_node_name_from_overall_json(const char *filename, const char *current_node_name, char *all_node_name[]);


int string_to_time_t(const char* time_buffer, time_t* parsed_time);
int time_t_to_string(time_t time_val, char* buffer, size_t buffer_size);

char* get_interface_type_by_index(int i);

int get_can_baud_rate_by_index(int i);

char *get_linked_node(int i);

int get_interface_cnt();
char* get_interface_name_by_index(int i);
char* get_linked_interface_name_by_index(int i);


char* get_initializer_name_by_index(int i);
char* get_closer_name_by_index(int i);


char* get_rs485_dev_path_by_index(int i);
char* get_ip_name_by_index(int i);

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


int update_communication_info_array(char* linked_node,char* interface_name,time_t updated_time,unsigned long tx,unsigned long rx,int current_round); //,char* error_ratio_value
void string_to_unsigned_long(const char* str, unsigned long* result);

int get_interface_index(const char* interface_name);
void printCertainCommucationInfo(const char* certain_interface_name);

void string_to_int(const char* str, int* result);

char* get_interface_status(const char* interface_name);
char* get_interface_mode(const char* interface_name);
char* get_interface_duplex_by_index(int i);

char* get_base_send_func_by_index(int i);

char* get_base_receive_func_by_index(int i);


char* get_interface_mode_by_index(int i);

int  get_rs485_baud_rate_by_index(int i);


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

char* get_status_chooser_by_index(int i);
char*  get_msg_generator_of_sender_by_index(int i);

void print_communication_info_array(const struct communication_info *array, int size);

void printAllCommucationInfo();
int read_communication_info_array_from_json(const char *filename);


#endif
