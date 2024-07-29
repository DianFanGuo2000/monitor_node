/******************************************************************************
 *File Name :   udp_Server.h
 *Copyright :   Zhuzhou CSR Times Electric Co.,Ltd. All Rights Reserved.
 *Create Date   :   2018/01/25
 *Description   :   generate message function file.
 *
 *REV1.0.0    Moyun   2018/01/25  File Create
*******************************************************************************/

/******************************************************************************
*    Multi-Include-Prevent Start Section
******************************************************************************/
#ifndef   _UDP_SERVER_H_
#define  _UDP_SERVER_H_

/******************************************************************************
*    Include File Section
******************************************************************************/

/******************************************************************************
*    Global Macro Define Section
******************************************************************************/

#include "mng_Interface.h"


#define DEVEICE_TYPE_LEN   (10)
#define UDP_SEND_DATA_LEN  (1000)
#define INET_ADDR_LEN (20)
#define UDP_MULTI_TTL_VAL  (128)


#define   RSEVTMSGCNTMAX  (200)



#define UDP_SEND_PORT    (9002)       /*the udp client send port*/
#define UDP_RECV_PORT    (9003)       /*the udp server receive port*/

#define UDP_RECV_CANFD1_PORT    (9004)       /*the udp server receive port*/
#define UDP_RECV_CANFD2_PORT    (9005)       /*the udp server receive port*/
#define UDP_RECV_CANFD3_PORT    (9006)       /*the udp server receive port*/




#define UDP_MAX_RECEIVE_LEN  (1432)


#define  UDP_CLENT_TO_IP_VALUE   "192.168.1.199"





typedef struct _UDP_RECV_STATISTIC
{
	
	UINT32 udpCanRecvNum;
	UINT32 udpCanFd1RecvNum;
	UINT32 udpCanFd2RecvNum;
	UINT32 udpCanFd3RecvNum;
}UDP_RECV_STATISTIC;

/******************************************************************************
*    Global Struct  Define Section
******************************************************************************/


extern STATUS udp_ServerTask ( void );

/******************************************************************************
*    Multi-Include-Prevent End Section
******************************************************************************/
#endif
