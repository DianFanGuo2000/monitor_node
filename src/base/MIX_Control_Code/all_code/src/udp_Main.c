
#include <stdio.h>

#include "vos_EdrSemaphore.h"


#define STATUS  int


extern INT32 vos_EdrSemInit(VOS_EDR_SEM_ID *Sem_PId,INT32 SemMode_I32,enum SEM_B_STATE InitValue);

extern STATUS udp_ServerRecvTask (void);
extern STATUS udp_ClientSendTask (void);
extern STATUS udp_sendTestTask(void);



VOS_EDR_SEM_ID g_udpRecvCanDataSem_ID;
VOS_EDR_SEM_ID g_udpRecvLifeDataSem_ID;



VOS_EDR_SEM_ID g_udpRecvCan1DataSem_ID;
VOS_EDR_SEM_ID g_udpRecvCan2DataSem_ID;
VOS_EDR_SEM_ID g_udpRecvCan3DataSem_ID;
VOS_EDR_SEM_ID g_udpRecvCan4DataSem_ID;
VOS_EDR_SEM_ID g_udpRecvCan5DataSem_ID;


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


void udpDataRecvLifeSemaphoreInit(void)
{
	STATUS St_I32 = ERROR;

	/*init the mem record  semaphore*/
	St_I32 = vos_EdrSemInit ( &g_udpRecvCanDataSem_ID, VOS_SEM_Q_PRIORITY, SEM_FULL );

	if ( ERROR == St_I32 )
	{
	    ( void ) printf ( "g_udpRecvCanDataSem_ID create error \n" );
	}
	
	
}






void udpDataRecvCanSemaphoreInit(void)
{
	STATUS St_I32 = ERROR;

	/*init the mem record  semaphore*/
	St_I32 = vos_EdrSemInit ( &g_udpRecvCanDataSem_ID, VOS_SEM_Q_PRIORITY, SEM_FULL );

	if ( ERROR == St_I32 )
	{
	    ( void ) printf ( "g_udpRecvCanDataSem_ID create error \n" );
	}
	
	
}




void udpDataRecvCan1SemaphoreInit(void)
{
	STATUS St_I32 = ERROR;

	/*init the mem record  semaphore*/
	St_I32 = vos_EdrSemInit ( &g_udpRecvCan1DataSem_ID, VOS_SEM_Q_PRIORITY, SEM_FULL );

	if ( ERROR == St_I32 )
	{
	    ( void ) printf ( "g_udpRecvCan1DataSem_ID create error \n" );
	}
	
	
}


void udpDataRecvCan2SemaphoreInit(void)
{
	STATUS St_I32 = ERROR;

	/*init the mem record  semaphore*/
	St_I32 = vos_EdrSemInit ( &g_udpRecvCan2DataSem_ID, VOS_SEM_Q_PRIORITY, SEM_FULL );

	if ( ERROR == St_I32 )
	{
	    ( void ) printf ( "g_udpRecvCan2DataSem_ID create error \n" );
	}
	
	
}


void udpDataRecvCan3SemaphoreInit(void)
{
	STATUS St_I32 = ERROR;

	/*init the mem record  semaphore*/
	St_I32 = vos_EdrSemInit ( &g_udpRecvCan3DataSem_ID, VOS_SEM_Q_PRIORITY, SEM_FULL );

	if ( ERROR == St_I32 )
	{
	    ( void ) printf ( "g_udpRecvCan3DataSem_ID create error \n" );
	}
	
	
}


void udpDataRecvCan4SemaphoreInit(void)
{
	STATUS St_I32 = ERROR;

	/*init the mem record  semaphore*/
	St_I32 = vos_EdrSemInit ( &g_udpRecvCan4DataSem_ID, VOS_SEM_Q_PRIORITY, SEM_FULL );

	if ( ERROR == St_I32 )
	{
	    ( void ) printf ( "g_udpRecvCan4DataSem_ID create error \n" );
	}
	
	
}


void udpDataRecvCan5SemaphoreInit(void)
{
	STATUS St_I32 = ERROR;

	/*init the mem record  semaphore*/
	St_I32 = vos_EdrSemInit ( &g_udpRecvCan5DataSem_ID, VOS_SEM_Q_PRIORITY, SEM_FULL );

	if ( ERROR == St_I32 )
	{
	    ( void ) printf ( "g_udpRecvCan5DataSem_ID create error \n" );
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


#if 0
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

#endif
