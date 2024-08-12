#ifndef ONE_INTERFACE_OPERATOR_H  
#define ONE_INTERFACE_OPERATOR_H  

#include <pthread.h>  
#include <stdio.h>  
#include <stdlib.h>  
#include "firmware.h"

extern char *status_chooser_transfer(char *status_chooser_name, const char *interface_name, const char *mode, const int current_round);
extern int msg_generator_transfer(char *msg_generator_name, const char *interface_name, const char *mode, const int current_round,char *msg);
extern int initializer_transfer(char *initializer_name, const char *interface_name);
extern int closer_transfer(char *closer_name, const char *interface_name);


extern void initialize_lock();

extern void destroy_lock();


typedef struct {  
    const char *interface_name;  
    const char *message;
} ThreadArgs;  

char *res_file_name;



void update_status_in_current_round(const char *updated_interface,const char *mode, int current_round);


void set_res_file_name(char *file_name);
int sync_communication_info(const char* center_interface_name);


char* generate_eth_reply(const char* msg);

void listen_upon_one_interface_in_one_time(char *linked_node, char *listened_interface);



void test_upon_one_interface_in_one_time(const char *test_interface,int packages_num);

void init_test_or_listen_record_arrays();
void free_test_or_listen_record_arrays();



#endif
