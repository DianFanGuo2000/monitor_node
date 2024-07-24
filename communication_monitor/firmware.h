#ifndef FIRMWARE_H  
#define FIRMWARE_H  
#include <stdlib.h> // For _ERROR (assuming it's defined somewhere)  
#include <string.h> // For strcmp  
#include <time.h> // For time functions  

#include "configures.h"
#include "base/eth_recv_send.h"
#include "info_manager.h"



#define _ERROR -1  
#define _SAME 1  
#define _NOT_SAME 2  
  




typedef char* (*ReplyGenerator)(const char* msg);  
int receive_and_reply(const char *source_interface,      
                      ReplyGenerator generate_reply,
                      long max_waiting_time);

int send_until_being_replied(const char *source_interface,     
                             const char *message,   
                             long max_waiting_time_in_one_sending_time, 
                             long max_waiting_time_in_all,     
                             int max_send_times,     
                             const char* expected_reply_str) ;



#endif
