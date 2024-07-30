/******************************************************************************
 *File Name :   udp_Client.h
 *Copyright :   Zhuzhou CSR Times Electric Co.,Ltd. All Rights Reserved.
 *Create Date   :   2018/01/25
 *Description   :   generate message function file.
 *
 *REV1.0.0    Moyun   2018/01/25  File Create
*******************************************************************************/

/******************************************************************************
*    Multi-Include-Prevent Start Section
******************************************************************************/
#ifndef   _UDP_CLIENT_H_
#define  _UDP_CLIENT_H_

#include "mng_Interface.h"


typedef struct _UDP_SEND_STATISTIC
{
	
	UINT32  udpQrecvRightNum;
	UINT32  udpQrecvErrorNum;
	UINT32  udpSendRightNum;
	UINT32  udpSendErrorNum;
	
}UDP_SEND_STATISTIC;




/******************************************************************************
*    Include File Section
******************************************************************************/
#define EDR_LINUX_VER 

#ifdef EDR_VXWORKS_VER

#endif

#ifdef EDR_LINUX_VER

#endif

/******************************************************************************
*    Global Macro Define Section
******************************************************************************/

/******************************************************************************
*    Global Struct  Define Section
******************************************************************************/


/******************************************************************************
*    Global Prototype Declare Section
******************************************************************************/


/******************************************************************************
*    Multi-Include-Prevent End Section
******************************************************************************/
#endif

