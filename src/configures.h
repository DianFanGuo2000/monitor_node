#ifndef CONFIGURES_H  
#define CONFIGURES_H  




#define RESULT_STRING_SIZE 20
#define BUFFER_SIZE 40000 // 40KB

#define MAX_WAITING_TIME_IN_ONE_ROUND 10 // s
#define TEST_BEIGIN_TIME "Tue Aug 26 18:31:15 2024" // s

#define PAKCAGES_NUM_ONE_TIME 20 // 20 pakcages one time


#define DOWNLOAD_ALL_COMMUNICATION_INFOS_LOCALLY 30
#define DO_NOTHING_WITH_THE_COMMUNICATION_INFOS 31
#define SYNC_THE_COMMUNICATION_INFOS 32




#define LISTENING_THREAD_NUM 1 // 20 pakcages one time
/*
这里线程数目智能设置为1，我开始设想给所有访问底层代码的线程用一把锁，结果发现有问题，需要给每个监听线程都动态
配上一把锁才行，但目前没什么必要，因为发送时间间隔很大，不需要多个监听线程轮询，所以我把底层代码中的一把锁给去掉
了，但是这个宏先不变，放着吧，后面有多线程监听轮询的需求可以再往上加
*/


#define SENDING_TIME_SPEC 200000000 //ns
#define WAITING_TIME_FOR_SENDING_AFTER_ONE_ROUND_CHANGING_POINT 1000000000 //ns

#define MAX_SYNC_COMMUNICATION_INFO_NUM_FOR_ONE_TIME 6


#define _ERROR -1
#define _SUCCESS 0




#define MAX_RES_LEN 2048 // 需要小于等于MAX_ETH_DATA_LENGTH
#define MAX_CAN_DATA_LENGTH 8
#define MAX_RS485_DATA_LENGTH 64
#define MAX_ETH_DATA_LENGTH 2048


#define MAX_IF_LEN 64


#define MAX_CAN_ID_STR_LEN 32
#define MAX_BAUD_RATE_STR_LEN 32
#define MAX_INTERFACES 100
#define RECEIVED_CAN_DATA_PACKAGE_SIZE 86
#define OFFSET_OF_TARGET_DATA_IN_RECEIVED_CAN_DATA_PACKAGE 16

#define INITIALIZE_TIME_SPEC 200000000 //ns

#define SYNC_PARTS_NUM 2
#define SYNC_TIMES 2

#define RESTART_CHECK_TIME_SPEC 600//s



#define ETH_P_SNMP 0x080C



#endif


