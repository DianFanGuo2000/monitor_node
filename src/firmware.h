#ifndef FIRMWARE_H  
#define FIRMWARE_H  
#include <stdlib.h> // For _ERROR (assuming it's defined somewhere)  
#include <string.h> // For strcmp  
#include <time.h> // For time functions  

#include "./base/eth_recv_send.h"
#include "./base/rs485_recv_send.h"
#include "./base/can_recv_send.h"


#include "./base/info_manager.h"
#include "configures.h"
#include "./base/MIX_Control_Code/all_code/src/can_app.c"



typedef void (*Dealer)(const char* located_node,const char* listened_interface, const char* msg);  

typedef struct {  
    Dealer deal_func;  
    char msg[MAX_MSG_LEN];
	char* linked_node;
	char* listened_interface;
} DealData; 

 



int receive_message(const char *linked_node,const char *source_interface,Dealer deal,long max_waiting_time);


int send_message(const char *source_interface,const char *message);

int init_basic_interface(int i);

int close_basic_interface(int i);

int set_status(const char *source_interface,const char *status);

void sync_status(const char *source_interface);
void fillMessageToRS485Len(const char *message, char *RS485MSG, int rs485_len);



#endif
