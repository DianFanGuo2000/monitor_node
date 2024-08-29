#ifndef ETH_RECV_SEND_IP_H
#define ETH_RECV_SEND_IP_H 

#include "../configures.h"


#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/ether.h>
#include <netinet/in.h>
#include <linux/if_packet.h>
#include <pthread.h>  

#define MIN(a, b) ((a) < (b) ? (a) : (b)) 


pthread_mutex_t eth_by_ip_addr_lock;


// 在程序初始化时创建锁  
void initialize_eth_by_ip_addr_lock();
  
// 在程序结束时销毁锁  
void destroy_eth_by_ip_addr_lock();


int receive_packet_by_ip_addr(const char *listened_ip_name,const char *listened_ip_addr,int listened_port_id, unsigned char *msg, long max_waiting_time);
int send_packet_by_ip_addr(const char *target_ip_name,const char *target_ip_addr, int target_port_id,const char *message);



#endif



