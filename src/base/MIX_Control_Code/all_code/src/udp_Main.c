
#include <stdio.h>

#include "vos_EdrSemaphore.h"


#define STATUS  int


extern INT32 vos_EdrSemInit(VOS_EDR_SEM_ID *Sem_PId,INT32 SemMode_I32,enum SEM_B_STATE InitValue);

extern STATUS udp_ServerRecvTask (void);
extern STATUS udp_ClientSendTask (void);
extern STATUS udp_sendTestTask(void);

VOS_EDR_SEM_ID g_udpRecvDataSem_ID;
VOS_EDR_SEM_ID g_udpRecvCanFd1DataSem_ID;
VOS_EDR_SEM_ID g_udpRecvCanFd2DataSem_ID;
VOS_EDR_SEM_ID g_udpRecvCanFd3DataSem_ID;



/******************************************************************************
 * Name:  UINT32 TrdpLinkConf(void)
 * Function: read the xml file
 * Input:  file name
 * return : non
 * Create by ningqiao  2013-4-1
 *******************************************************************************/


void udpDataRecvSemaphoreInit(void)
{
	STATUS St_I32 = ERROR;

	/*init the mem record  semaphore*/
	St_I32 = vos_EdrSemInit ( &g_udpRecvDataSem_ID, VOS_SEM_Q_PRIORITY, SEM_FULL );

	if ( ERROR == St_I32 )
	{
	    ( void ) printf ( "g_udpRecvDataSem_ID create error \n" );
	}
	
	
}


void udpDataCanFd1RecvSemaphoreInit(void)
{
	STATUS St_I32 = ERROR;

	/*init the mem record  semaphore*/
	St_I32 = vos_EdrSemInit ( &g_udpRecvCanFd1DataSem_ID, VOS_SEM_Q_PRIORITY, SEM_FULL );

	if ( ERROR == St_I32 )
	{
	    ( void ) printf ( "g_udpRecvCanFd1DataSem_ID create error \n" );
	}
	
	
}


void udpDataCanFd2RecvSemaphoreInit(void)
{
	STATUS St_I32 = ERROR;

	/*init the mem record  semaphore*/
	St_I32 = vos_EdrSemInit ( &g_udpRecvCanFd2DataSem_ID, VOS_SEM_Q_PRIORITY, SEM_FULL );

	if ( ERROR == St_I32 )
	{
	    ( void ) printf ( "g_udpRecvCanFd2DataSem_ID create error \n" );
	}
	
	
}

void udpDataCanFd3RecvSemaphoreInit(void)
{
	STATUS St_I32 = ERROR;

	/*init the mem record  semaphore*/
	St_I32 = vos_EdrSemInit ( &g_udpRecvCanFd3DataSem_ID, VOS_SEM_Q_PRIORITY, SEM_FULL );

	if ( ERROR == St_I32 )
	{
	    ( void ) printf ( "g_udpRecvCanFd3DataSem_ID create error \n" );
	}
	
	
}



void udp_Main(void)
{
	
    udpDataRecvSemaphoreInit();
	udpDataCanFd1RecvSemaphoreInit();
	udpDataCanFd2RecvSemaphoreInit();
	udpDataCanFd3RecvSemaphoreInit();
    udp_ServerRecvTask();/*process data recv */
    udp_ClientSendTask();/*process data send */
    
    //udp_sendTestTask();
	
}
