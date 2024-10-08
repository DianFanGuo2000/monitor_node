#ifndef ETH_RECV_SEND_XY_H
#define ETH_RECV_SEND_XY_H 

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


pthread_mutex_t eth_lock_xy;


// 在程序初始化时创建锁  
void initialize_eth_lock_xy();
  
// 在程序结束时销毁锁  
void destroy_eth_lock_xy();


int receive_packet_xy(int sockfd, unsigned char *msg,long max_waiting_time);
int send_packet_xy(int sockfd, const struct sockaddr_ll* sock_addr_value_addr, const char *message, const char *ether_shost, const char *ether_dhost);



#endif



