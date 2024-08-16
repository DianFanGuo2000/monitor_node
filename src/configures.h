#ifndef CONFIGURES_H  
#define CONFIGURES_H  




#define RESULT_STRING_SIZE 20
#define BUFFER_SIZE 30000 // 30KB

#define MAX_WAITING_TIME_IN_ONE_ROUND 8 // s
#define TEST_BEIGIN_TIME "Tue Jul 23 18:31:15 2024" // s

#define PAKCAGES_NUM_ONE_TIME 20 // 20 pakcages one time



#define LISTENING_THREAD_NUM 4 // 20 pakcages one time


#define SENDING_TIME_SPEC 200000000 //ns
#define WAITING_TIME_FOR_SENDING_AFTER_ONE_ROUND_CHANGING_POINT 1000000000 //ns


#define _ERROR -1
#define _SUCCESS 0
 
#define MAX_MSG_LEN 512
#define MAX_IF_LEN 64

#define MAX_CAN_ID_STR_LEN 32
#define MAX_BAUD_RATE_STR_LEN 32
#define MAX_INTERFACES 100


#define ETH_P_SNMP 0x080C



#define RECEIVED_CAN_DATA_PACKAGE_SIZE 86
#define OFFSET_OF_TARGET_DATA_IN_RECEIVED_CAN_DATA_PACKAGE 16
#define MAX_CAN_DATA_LENGTH 8



#endif


