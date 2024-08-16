#ifndef INITIALIZER_CLOSER_LIB_H  
#define INITIALIZER_CLOSER_LIB_H  

#include "configures.h"
#include "base/info_manager.h"
#include "./base/rs485_recv_send.h"
#include "./base/can_fpu_recv_send.h"
#include "./base/can_gpu_recv_send.h"
#include <pthread.h>


char *initializer_name_array[] = {"eth_initializer_normal", "rs485_initializer_normal", "can_fpu_initializer_normal", "can_gpu_initializer_normal"};  
int initializer_num = 4;

char *closer_name_array[] = {"eth_closer_normal", "rs485_closer_normal", "can_fpu_closer_normal", "can_gpu_closer_normal"};  
int closer_num = 4;


int initializer_transfer(char *initializer_name, const char *interface_name);
int closer_transfer(char *closer_name, const char *interface_name);


int eth_initializer_normal(const char *interface_name);

int rs485_initializer_normal(const char *interface_name);

int can_fpu_initializer_normal(const char *interface_name);
int can_gpu_initializer_normal(const char *interface_name);

int eth_closer_normal(const char *interface_name);

int rs485_closer_normal(const char *interface_name);

int can_fpu_closer_normal(const char *interface_name);
int can_gpu_closer_normal(const char *interface_name);






#endif

