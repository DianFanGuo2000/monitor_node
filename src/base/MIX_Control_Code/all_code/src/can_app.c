
#include  <stdio.h>
#include "vos_EdrMessage.h"
#include "mng_Interface.h"
#include "vos_EdrTask.h"
#include "udp_Server.h"
#include "vos_EdrSemaphore.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <linux/ioctl.h>
#include <unistd.h>




extern void getUdpSendStatistic(UINT32 *sendQrecvRightNum,UINT32 *sendQrecvErrorNum,UINT32 *sendUdpRightNum,UINT32 *sendUdpErrorNum);
extern void getUdpRecvStatistic(UINT32 *udpCan1RecvNum,UINT32 *udpCan2RecvNum,UINT32 *udpCan3RecvNum,UINT32 *udpCan4RecvNum,UINT32 *udpCan5RecvNum,UINT32 *udpCanFd1RecvNum,UINT32 *udpCanFd2RecvNum,UINT32 *udpCanFd3RecvNum);
extern STATUS vos_EdrMsgQSend(VOS_EDR_MSG_Q_ID MsgQId,char *Buffer_pC8,UINT32 Bytes_U32, INT32 TimeOut_I32,INT32 Priority_I32);


extern VOS_EDR_SEM_ID g_udpRecvCanDataSem_ID;
extern VOS_EDR_SEM_ID g_udpRecvLifeDataSem_ID;


extern VOS_EDR_SEM_ID g_udpRecvCan1DataSem_ID;
extern VOS_EDR_SEM_ID g_udpRecvCan2DataSem_ID;
extern VOS_EDR_SEM_ID g_udpRecvCan3DataSem_ID;
extern VOS_EDR_SEM_ID g_udpRecvCan4DataSem_ID;
extern VOS_EDR_SEM_ID g_udpRecvCan5DataSem_ID;


extern VOS_EDR_SEM_ID g_udpRecvCanFd1DataSem_ID;
extern VOS_EDR_SEM_ID g_udpRecvCanFd2DataSem_ID;
extern VOS_EDR_SEM_ID g_udpRecvCanFd3DataSem_ID;



extern INT32  vos_EdrSemPost(VOS_EDR_SEM_ID* Sem_pId);
extern INT32 vos_EdrSemWait(VOS_EDR_SEM_ID *Sem_pId,INT32 Timeout_I32);


extern void udpDataRecvCan1SemaphoreInit(void);
extern void udpDataRecvCan2SemaphoreInit(void);
extern void udpDataRecvCan3SemaphoreInit(void);
extern void udpDataRecvCan4SemaphoreInit(void);
extern void udpDataRecvCan5SemaphoreInit(void);


extern STATUS udp_ClientSendTask (void);


extern STATUS udp_ServerCanProcessRecvMain (void);
extern STATUS udp_ServerLifeProcessRecvMain (void);


extern STATUS udp_ServerCan1ProcessRecvMain (void);
extern STATUS udp_ServerCan2ProcessRecvMain (void);
extern STATUS udp_ServerCan3ProcessRecvMain (void);
extern STATUS udp_ServerCan4ProcessRecvMain (void);
extern STATUS udp_ServerCan5ProcessRecvMain (void);
//extern STATUS udp_ServerCanFd1ProcessRecvMain (void);
//extern STATUS udp_ServerCanFd2ProcessRecvMain (void);
//extern STATUS udp_ServerCanFd3ProcessRecvMain (void);

extern STATUS udp_ServerProcessCanFd1RecvMain (void);
extern STATUS udp_ServerProcessCanFd2RecvMain (void);
extern STATUS udp_ServerProcessCanFd3RecvMain (void);




extern INT32 recvUdpCanDataFunc(char *data,UINT32 dataLength);
extern INT32 recvUdpCanLifeFunc(char *data,UINT32 dataLength);

extern INT32 recvUdpCan1DataFunc(char *data,UINT32 dataLength);
extern INT32 recvUdpCan2DataFunc(char *data,UINT32 dataLength);
extern INT32 recvUdpCan3DataFunc(char *data,UINT32 dataLength);
extern INT32 recvUdpCan4DataFunc(char *data,UINT32 dataLength);
extern INT32 recvUdpCan5DataFunc(char *data,UINT32 dataLength);
extern INT32 recvUdpCanFd1DataFunc(char *data,UINT32 dataLength);
extern INT32 recvUdpCanFd2DataFunc(char *data,UINT32 dataLength);
extern INT32 recvUdpCanFd3DataFunc(char *data,UINT32 dataLength);



#define     USED_CHANNEL_NUM      (4)

#define     CFG_REQUEST_HEAD      (0x11)
#define     CFG_RESPONSE_HEAD     (0x33)

#define     CFG_SUCESS_FLAG       (0x55)
#define     CFG_FAILURE_FLAG        (0xAA)

#define     PROCESS_DATA_MCM_OUT_HEAD   (0x1)
#define     PROCESS_DATA_IN_MCM_HEAD    (0x2)

#define     TASK_DELAY_TIME         (5)


#define     CHANNEL_ONE          (1)
#define     CHANNEL_TWO          (2)
#define     CHANNEL_THREE        (3)
#define     CHANNEL_FOUR         (4)
#define     CHANNEL_FIVE        (5)
#define     CHANNEL_SIX         (6)


#define     CAN_MODE            (0x01)
#define     CANFD_MODE            (0x02)


#define     CAN_MODE_STD            (0x01)
#define     CANFD_MODE_STD            (0x02)
#define     CAN_MODE_EXT            (0x03)
#define     CANFD_MODE_EXT            (0x04)

#define     CHANNEL_CONFG_SUCESS   (1)


#define     CHANNEL_CAN_DATA_LENGTH      (8)
#define     CHANNEL_CANFD_DATA_LENGTH    (64)


#define     DATA_OFFSET       (14)

#define     CAN_DATA_OFFSET       (16)


#define     GET_DATA_OK       (1)
#define     GET_DATA_ERROR    (0)


UINT8 g_recvCanDataArray[1432] = {0};
UINT8 g_recvCanLifeArray[1432] = {0};



UINT8 g_recvCan1DataArray[1432] = {0};
UINT8 g_recvCan2DataArray[1432] = {0};
UINT8 g_recvCan3DataArray[1432] = {0};
UINT8 g_recvCan4DataArray[1432] = {0};
UINT8 g_recvCan5DataArray[1432] = {0};





UINT8 g_recvCanFd1DataArray[1432] = {0};

UINT8 g_recvCanFd2DataArray[1432] = {0};

UINT8 g_recvCanFd3DataArray[1432] = {0};



#define CANFD_CHANNEL_ONE     (1)
#define CANFD_CHANNEL_TWO     (2)
#define CANFD_CHANNEL_THREE   (3)



extern VOS_EDR_MSG_Q_ID     g_RsDataToUdpMsgID;


extern void udp_Main(void);


UINT8 g_sendDataLife =0;

UINT8 g_array[64] = {0};


UINT32 g_initInterTimes = 0;



UINT32 g_channelOneTestSucess = 0;
UINT32 g_channelOneTestError = 0;

UINT32 g_channelTwoTestSucess = 0;
UINT32 g_channelTwoTestError = 0;

UINT32 g_channelThreeTestSucess = 0;
UINT32 g_channelThreeTestError = 0;

UINT32 g_channelFourTestSucess = 0;
UINT32 g_channelFourTestError = 0;


//extern CAN_CHANNEL_DATA  g_recvCanData[CHANNEL_NUM_MAX][CANID_NUM_MAX];

//extern UINT16 g_recvDifferentCanIdStatistic[CHANNEL_NUM_MAX];

/*return 0 sucess; -1 error */

#pragma  pack(1)



typedef struct _CFG_INIT_INFO
{
   long  msgType;
   UINT32 cfgRequestFlag;
   UINT8 portNum;
   UINT8 portId;
   UINT8 mode;
   UINT32 bpsRate;
   UINT32 canFdRate; 
}CFG_INIT_INFO;



typedef struct _CFG_FEEDABCK_RECV_INFO
{
	UINT32 cfgResponse;
	UINT8 portNum;
	UINT8 portId;
	UINT8 portCfgstatus;
	
}CFG_FEEDABCK_RECV_INFO;



typedef struct _CAN_PROCESS
{

	UINT32 dataHead;
        UINT8 temp;
        UINT8 reserver;
    
	UINT8 portId;
	UINT8 frameDataLength;
	UINT16 reserver1;
	UINT32 canId;
	UINT8 dataArray[64];

}CAN_PROCESS;


typedef struct _CAN_PROCESS_DATA
{
	
      long processType;
      CAN_PROCESS processData;
}CAN_PROCESS_DATA;


#if 0

typedef struct _CAN_NEW_PROCESS
{

	UINT32 dataHead;
    UINT8 temp;
    UINT8 reserver;
    
	UINT8 portId;
	UINT8 frameDataLength;
	UINT16 reserver1;
	UINT32 canId;
	UINT8 dataArray[64];

}CAN_NEW_PROCESS;


typedef struct _CAN_NEW_PROCESS_DATA
{
	
      long processType;
      CAN_NEW_PROCESS processData;
}CAN_NEW_PROCESS_DATA;


#endif


#pragma  pack()




INT32 appMcuStatRecv(char *recvDataPointer,UINT32 recvDataLength)
{
    UINT16 i=0;
	UINT32 *retPointer = NULL;  
	vos_EdrSemWait(&g_udpRecvLifeDataSem_ID, WAIT_FOREVER);
	retPointer = memcpy((void *)recvDataPointer,g_recvCanLifeArray,recvDataLength);
	memset(g_recvCanLifeArray,0x0,sizeof(g_recvCanLifeArray));
	vos_EdrSemPost(&g_udpRecvLifeDataSem_ID); 	
	if(retPointer == NULL)
	{
		return ERROR;
	}
	
	return OK;
	
}





INT32 appCanCfgDataRecv(UINT32 com,char *recvDataPointer,UINT32 recvDataLength )
{
    UINT16 i=0;
	UINT32 *retPointer = NULL;  
	switch(com)
		{


		   case 1:
    vos_EdrSemWait(&g_udpRecvCanFd1DataSem_ID, WAIT_FOREVER);
	retPointer = memcpy((void *)recvDataPointer,g_recvCanFd1DataArray,recvDataLength);
	memset(g_recvCanFd1DataArray,0x0,sizeof(g_recvCanFd1DataArray));
	vos_EdrSemPost(&g_udpRecvCanFd1DataSem_ID); 
		   	
		   	break;



			case 2:
	vos_EdrSemWait(&g_udpRecvCan3DataSem_ID, WAIT_FOREVER);
	retPointer = memcpy((void *)recvDataPointer,g_recvCan3DataArray,recvDataLength);
	memset(g_recvCan3DataArray,0x0,sizeof(g_recvCan3DataArray));
	vos_EdrSemPost(&g_udpRecvCan3DataSem_ID); 
		   	
		   	break;



			case 3:
	vos_EdrSemWait(&g_udpRecvCan3DataSem_ID, WAIT_FOREVER);
	retPointer = memcpy((void *)recvDataPointer,g_recvCan2DataArray,recvDataLength);
	memset(g_recvCan2DataArray,0x0,sizeof(g_recvCan2DataArray));
	vos_EdrSemPost(&g_udpRecvCan3DataSem_ID); 
		   	
		   	break;



			case 4:
	vos_EdrSemWait(&g_udpRecvCanFd2DataSem_ID, WAIT_FOREVER);
	retPointer = memcpy((void *)recvDataPointer,g_recvCanFd2DataArray,recvDataLength);
	memset(g_recvCanFd2DataArray,0x0,sizeof(g_recvCanFd2DataArray));
	vos_EdrSemPost(&g_udpRecvCanFd2DataSem_ID); 
		   	
		   	break;



			case 5:
	vos_EdrSemWait(&g_udpRecvCanFd3DataSem_ID, WAIT_FOREVER);
	retPointer = memcpy((void *)recvDataPointer,g_recvCanFd3DataArray,recvDataLength);
	memset(g_recvCanFd3DataArray,0x0,sizeof(g_recvCanFd3DataArray));
	vos_EdrSemPost(&g_udpRecvCanFd3DataSem_ID); 
		   	
		   	break;



			case 6:
    vos_EdrSemWait(&g_udpRecvCan5DataSem_ID, WAIT_FOREVER);
	retPointer = memcpy((void *)recvDataPointer,g_recvCan5DataArray,recvDataLength);
	memset(g_recvCan5DataArray,0x0,sizeof(g_recvCan5DataArray));
	vos_EdrSemPost(&g_udpRecvCan5DataSem_ID); 
		   	
		   	break;



			case 7:
	vos_EdrSemWait(&g_udpRecvCan4DataSem_ID, WAIT_FOREVER);
	retPointer = memcpy((void *)recvDataPointer,g_recvCan4DataArray,recvDataLength);
	memset(g_recvCan4DataArray,0x0,sizeof(g_recvCan4DataArray));
	vos_EdrSemPost(&g_udpRecvCan4DataSem_ID); 
		   	
		   	break;



			case 8:
    vos_EdrSemWait(&g_udpRecvCan1DataSem_ID, WAIT_FOREVER);
	retPointer = memcpy((void *)recvDataPointer,g_recvCan1DataArray,recvDataLength);
	memset(g_recvCan1DataArray,0x0,sizeof(g_recvCan1DataArray));
	vos_EdrSemPost(&g_udpRecvCan1DataSem_ID); 
		   	
		   	break;






		}
	
	if(retPointer == NULL)
	{
		return ERROR;
	}
	
	return OK;
	
}





INT32 appCan1DataRecv(char *recvDataPointer,UINT32 recvDataLength,INT32 Timeout_I32)
{
    UINT16 i=0;
	UINT32 *retPointer = NULL;  
	vos_EdrSemWait(&g_udpRecvCan1DataSem_ID, Timeout_I32);
	retPointer = memcpy((void *)recvDataPointer,g_recvCan1DataArray,recvDataLength);
	memset(g_recvCan1DataArray,0x0,sizeof(g_recvCan1DataArray));
	vos_EdrSemPost(&g_udpRecvCan1DataSem_ID); 	
	if(retPointer == NULL)
	{
		return ERROR;
	}
	
	return OK;
	
}


INT32 appCan2DataRecv(char *recvDataPointer,UINT32 recvDataLength,INT32 Timeout_I32)
{
    UINT16 i=0;
	UINT32 *retPointer = NULL;  
	vos_EdrSemWait(&g_udpRecvCan2DataSem_ID, Timeout_I32);
	retPointer = memcpy((void *)recvDataPointer,g_recvCan2DataArray,recvDataLength);
	memset(g_recvCan2DataArray,0x0,sizeof(g_recvCan2DataArray));
	vos_EdrSemPost(&g_udpRecvCan2DataSem_ID); 	
	if(retPointer == NULL)
	{
		return ERROR;
	}
	
	return OK;
	
}


INT32 appCan3DataRecv(char *recvDataPointer,UINT32 recvDataLength,INT32 Timeout_I32)
{
    UINT16 i=0;
	UINT32 *retPointer = NULL;  
	vos_EdrSemWait(&g_udpRecvCan3DataSem_ID, Timeout_I32);
	retPointer = memcpy((void *)recvDataPointer,g_recvCan3DataArray,recvDataLength);
	memset(g_recvCan3DataArray,0x0,sizeof(g_recvCan3DataArray));
	vos_EdrSemPost(&g_udpRecvCan3DataSem_ID); 	
	if(retPointer == NULL)
	{
		return ERROR;
	}
	
	return OK;
	
}


INT32 appCan4DataRecv(char *recvDataPointer,UINT32 recvDataLength,INT32 Timeout_I32)
{
    UINT16 i=0;
	UINT32 *retPointer = NULL;  
	vos_EdrSemWait(&g_udpRecvCan4DataSem_ID, Timeout_I32);
	retPointer = memcpy((void *)recvDataPointer,g_recvCan4DataArray,recvDataLength);
	memset(g_recvCan4DataArray,0x0,sizeof(g_recvCan4DataArray));
	vos_EdrSemPost(&g_udpRecvCan4DataSem_ID); 	
	if(retPointer == NULL)
	{
		return ERROR;
	}
	
	return OK;
	
}


INT32 appCan5DataRecv(char *recvDataPointer,UINT32 recvDataLength,INT32 Timeout_I32)
{
    UINT16 i=0;
	UINT32 *retPointer = NULL;  
	vos_EdrSemWait(&g_udpRecvCan5DataSem_ID, Timeout_I32);
	retPointer = memcpy((void *)recvDataPointer,g_recvCan5DataArray,recvDataLength);
	memset(g_recvCan5DataArray,0x0,sizeof(g_recvCan5DataArray));
	vos_EdrSemPost(&g_udpRecvCan5DataSem_ID); 	
	if(retPointer == NULL)
	{
		return ERROR;
	}
	
	return OK;
	
}













INT32 appCanFd1DataRecv(char *recvDataPointer,UINT32 recvDataLength,INT32 Timeout_I32)
{
    UINT16 i=0;
	UINT32 *retPointer = NULL;
    
	vos_EdrSemWait(&g_udpRecvCanFd1DataSem_ID, Timeout_I32);
	retPointer = memcpy((void *)recvDataPointer,g_recvCanFd1DataArray,recvDataLength);
	memset(g_recvCanFd1DataArray,0x0,sizeof(g_recvCanFd1DataArray));
	vos_EdrSemPost(&g_udpRecvCanFd1DataSem_ID);

	if(retPointer == NULL)
	{
		return ERROR;
	}
	
	return OK;
	
}

INT32 appCanFd2DataRecv(char *recvDataPointer,UINT32 recvDataLength,INT32 Timeout_I32)
{
    UINT16 i=0;
	UINT32 *retPointer = NULL;   
	vos_EdrSemWait(&g_udpRecvCanFd2DataSem_ID, Timeout_I32);
	retPointer = memcpy((void *)recvDataPointer,g_recvCanFd2DataArray,recvDataLength);
	memset(g_recvCanFd2DataArray,0x0,sizeof(g_recvCanFd2DataArray));
	vos_EdrSemPost(&g_udpRecvCanFd2DataSem_ID);
	
	if(retPointer == NULL)
	{
		return ERROR;
	}
	
	return OK;
	
}

INT32 appCanFd3DataRecv(char *recvDataPointer,UINT32 recvDataLength,INT32 Timeout_I32)
{
    UINT16 i=0;
	UINT32 *retPointer = NULL;

    
	vos_EdrSemWait(&g_udpRecvCanFd3DataSem_ID, Timeout_I32);
	retPointer = memcpy((void *)recvDataPointer,g_recvCanFd3DataArray,recvDataLength);
	memset(g_recvCanFd3DataArray,0x0,sizeof(g_recvCanFd3DataArray));
	vos_EdrSemPost(&g_udpRecvCanFd3DataSem_ID);
	
	if(retPointer == NULL)
	{
		return ERROR;
	}
	
	return OK;
	
}






INT32 appCanFdDataRecv(UINT32 com,char *recvDataPointer,UINT32 recvDataLength,INT32 Timeout_I32)
{
	STATUS retValue = 0;

	if(com == 1)
	{
	    retValue = appCanFd1DataRecv(recvDataPointer,recvDataLength,Timeout_I32);
	}
	else if(com == 4)
	{
	    retValue = appCanFd2DataRecv(recvDataPointer,recvDataLength,Timeout_I32);
	}
	else if(com == 5)
	{
	    retValue = appCanFd3DataRecv(recvDataPointer,recvDataLength,Timeout_I32);
	}
	else
	{
	    retValue = 2;      
	    printf("canfd channel value is error \n");

	}
	
        return  retValue;

}







INT32 appCanDataRecv(UINT32 com,char *recvDataPointer,UINT32 recvDataLength,INT32 Timeout_I32)
{
	STATUS retValue = 0;

    if(recvDataPointer == NULL)
   	{
          retValue = 3;
          return retValue;
   	}

	if(com == 1)
	{
	    retValue = appCanFd1DataRecv(recvDataPointer,recvDataLength,Timeout_I32);
	}
	else if(com == 2)
	{
	    retValue = appCan2DataRecv(recvDataPointer,recvDataLength,Timeout_I32);
	}
	else if(com == 3)
	{
	    retValue = appCanFd3DataRecv(recvDataPointer,recvDataLength,Timeout_I32);
	}	
        else if(com == 4)
	{
	    retValue = appCan4DataRecv(recvDataPointer,recvDataLength,Timeout_I32);
	}
	else if(com == 5)
	{
	    retValue = appCanFd2DataRecv(recvDataPointer,recvDataLength,Timeout_I32);
	}
	else if(com == 6)
	{
	    retValue = appCan3DataRecv(recvDataPointer,recvDataLength,Timeout_I32);
	}
	else if(com == 7)
	{
	    retValue = appCan1DataRecv(recvDataPointer,recvDataLength,Timeout_I32);
	}
	else if(com == 8)
	{
	    retValue = appCan5DataRecv(recvDataPointer,recvDataLength,Timeout_I32);
	}
	else 
	{
	    retValue = 2;      
	    printf("can channel value is error \n");
	}
    return  retValue;
}







STATUS cfgResponseRecv(UINT8 com)
{
	
	CFG_FEEDABCK_RECV_INFO  feedBackInfoRecv = {0};
	STATUS retValue = 0;
	UINT8 i = 0;
	UINT8 cfgSucessCount = 0;
        retValue = appCanCfgDataRecv(com,(char *)&feedBackInfoRecv,sizeof(CFG_FEEDABCK_RECV_INFO));
	if(retValue == OK)
	{
		if((feedBackInfoRecv.cfgResponse = CFG_RESPONSE_HEAD))
		{
			if(feedBackInfoRecv.portId == com)
			{
				if(feedBackInfoRecv.portCfgstatus == CFG_SUCESS_FLAG)
				{
					return OK;
				}
			}	
		}	
	}
	
	return ERROR;
	
}





void initCfg(void)
{

STATUS st = OK;
udpDataRecvCanSemaphoreInit();


st = vos_EdrTaskSpawn ( "t_udpCan", 18, VOS_VX_FP_TASK, 40960, ( FUNCPTR ) udp_ServerCanProcessRecvMain,( INT32 ) NULL );

if ( st == ERROR )
{
	printf ( "udp_server Can process task create failed !\n" );

}

setUdpServerCanCallBackFunc(recvUdpCanDataFunc);


}





void initCan1(void)
{

STATUS st = OK;


udpDataRecvCan1SemaphoreInit();


st = vos_EdrTaskSpawn ( "t_udpCan1", 23, VOS_VX_FP_TASK, 40960, ( FUNCPTR ) udp_ServerCan1ProcessRecvMain,( INT32 ) NULL );

if ( st == ERROR )
{
	printf ( "udp_server Can1 process task create failed !\n" );

}

setUdpServerCan1CallBackFunc(recvUdpCan1DataFunc);


}

void initCan2(void)
{

STATUS st = OK;


udpDataRecvCan2SemaphoreInit();


st = vos_EdrTaskSpawn ( "t_udpCan2", 24, VOS_VX_FP_TASK, 40960, ( FUNCPTR ) udp_ServerCan2ProcessRecvMain,( INT32 ) NULL );

if ( st == ERROR )
{
	printf ( "udp_server Can2 process task create failed !\n" );

}
setUdpServerCan2CallBackFunc(recvUdpCan2DataFunc);


}

void initCan3(void)
{
STATUS st = OK;

udpDataRecvCan3SemaphoreInit();


st = vos_EdrTaskSpawn ( "t_udpCan3", 25, VOS_VX_FP_TASK, 40960, ( FUNCPTR ) udp_ServerCan3ProcessRecvMain,( INT32 ) NULL );

if ( st == ERROR )
{
	printf ( "udp_server Can3 process task create failed !\n" );

}

setUdpServerCan3CallBackFunc(recvUdpCan3DataFunc);


}


void initCan4(void)
{
STATUS st = OK;

udpDataRecvCan4SemaphoreInit();


st = vos_EdrTaskSpawn ( "t_udpCan4", 26, VOS_VX_FP_TASK, 40960, ( FUNCPTR ) udp_ServerCan4ProcessRecvMain,( INT32 ) NULL );

if ( st == ERROR )
{
	printf ( "udp_server Can4 process task create failed !\n" );

}

setUdpServerCan4CallBackFunc(recvUdpCan4DataFunc);


}


void initCan5(void)
{

STATUS st = OK;
udpDataRecvCan5SemaphoreInit();


st = vos_EdrTaskSpawn ( "t_udpCan5", 27, VOS_VX_FP_TASK, 40960, ( FUNCPTR ) udp_ServerCan5ProcessRecvMain,( INT32 ) NULL );

if ( st == ERROR )
{
	printf ( "udp_server Can5 process task create failed !\n" );

}

setUdpServerCan5CallBackFunc(recvUdpCan5DataFunc);


}



void initCanFd1(void)
{

STATUS st = OK;
printf("init semaphore start \r\n");
udpDataCanFd1RecvSemaphoreInit();

printf("init semaphore end \r\n");
st = vos_EdrTaskSpawn ( "t_udpCanFd1", 28, VOS_VX_FP_TASK, 40960, ( FUNCPTR ) udp_ServerProcessCanFd1RecvMain,( INT32 ) NULL );

if ( st == ERROR )
{
	printf ( "udp_server CanFd1 process task create failed !\n" );

}
else
{


        printf ( "udp_server CanFd1 process task create success !\n" );

}

printf ( "setUdpServerCanFd1CallBackFunc create start !\n" );
setUdpServerCanFd1CallBackFunc(recvUdpCanFd1DataFunc);
printf ( "setUdpServerCanFd1CallBackFunc create end !\n" );

}


void initCanFd2(void)
{

STATUS st = OK;
udpDataCanFd2RecvSemaphoreInit();


st = vos_EdrTaskSpawn ( "t_udpCanFd2", 29, VOS_VX_FP_TASK, 40960, ( FUNCPTR ) udp_ServerProcessCanFd2RecvMain,( INT32 ) NULL );

if ( st == ERROR )
{
	printf ( "udp_server CanFd2 process task create failed !\n" );

}

setUdpServerCanFd2CallBackFunc(recvUdpCanFd2DataFunc);


}



void initCanFd3(void)
{

STATUS st = OK;
udpDataCanFd3RecvSemaphoreInit();


st = vos_EdrTaskSpawn ( "t_udpCanFd3", 30, VOS_VX_FP_TASK, 40960, ( FUNCPTR ) udp_ServerProcessCanFd3RecvMain,( INT32 ) NULL );

if ( st == ERROR )
{
	printf ( "udp_server CanFd3 process task create failed !\n" );

}

setUdpServerCanFd3CallBackFunc(recvUdpCanFd3DataFunc);


}







void initComUdpServer(UINT32 com)
{
    switch(com)
    	{

		case 1:
			initCanFd1();
			break;
	    case 6:
			initCan3();
			break;
		case 2:
			initCan2();
			break;
	    case 5:
			initCanFd2();
			break;
		case 3:
			initCanFd3();
			break;
	    case 8:
			initCan5();
			break;
		case 4:
			initCan4();
			break;
	    case 7:
			initCan1();
			break;
    	}

}



void initUdpClient(UINT32 com)
{

  switch(com)
	  {
  
	  case 1:
		  udp_ClientSendTask();
		  break;
	  case 2:
		  udp_ClientSendCan2Task();
		  break;
	  case 3:
		  udp_ClientSendCan3Task();
		  break;
	  case 4:
		  udp_ClientSendCan4Task();
		  break;
	  case 5:
		 udp_ClientSendCan5Task();
		  break;
	  case 6:
		  udp_ClientSendCan6Task();
		  break;
	  case 7:
		  udp_ClientSendCan7Task();
		  break;
	  case 8:
		  udp_ClientSendCan8Task();
		  break;
	  }


  
}

void initComUdpCfgServer(void)
{
    initCfg();
}

void initLifeServer(void)
{
STATUS st = OK;
udpDataRecvLifeSemaphoreInit();


st = vos_EdrTaskSpawn ( "t_udpCanLife", 32, VOS_VX_FP_TASK, 40960, ( FUNCPTR ) udp_ServerLifeProcessRecvMain,( INT32 ) NULL );

if ( st == ERROR )
{
	printf ( "t_udpCanLife Can process task create failed !\n" );

}

setUdpServerCanLifeCallBackFunc(recvUdpCanLifeFunc);





}




/*1 4: CANfd  ;  2 3 CAN*/
STATUS comCfgInit(UINT32 com,UINT32 mode,UINT32 bpsRate)
{
	
	CFG_INIT_INFO cfgParameterSend ={0};
	INT32 retValue = ERROR; 
	INT32 num = 0;


	//g_initInterTimes++;
        if((com <1) ||(com > 8))
	{
             printf("com value is over !!\r\n");
             retValue = ERROR;
             return retValue;
	}

        if((mode != 1)&&(mode != 3))
	{
             printf("mode value is error !!\r\n");
             retValue = ERROR;
	     return retValue;
	}

        if((bpsRate != 125)&&(bpsRate != 250)&&(bpsRate != 500)&&(bpsRate != 1000))
	{
             printf("bpsRate value is error !!\r\n");
             retValue = ERROR;
             return retValue;
	}
    
        printf("init com udp server start ! \r\n");
	initComUdpServer(com);
        printf("init com udp client start ! \r\n");
	initUdpClient(com);

	//if(g_initInterTimes == 1)
	//{
	//  initComUdpCfgServer();
	//  initLifeServer()
	//}

	
	cfgParameterSend.msgType = 0x11223344;
	cfgParameterSend.cfgRequestFlag = CFG_REQUEST_HEAD;
	cfgParameterSend.portNum = 1;
	
	
	cfgParameterSend.mode =(UINT8) mode;/*1 can stand mode ;2 canfd stand mode;3 can ext mode ;4 canfd ext mode*/
	cfgParameterSend.portId = (UINT8)com; /*1 2 3 4 */
	cfgParameterSend.bpsRate = bpsRate;
	//cfgParameterSend.canFdRate = canFdBpsRate;
	
    printf("send start ! \r\n");
	
	retValue = appCanDataSendFunc(com,(char *)&cfgParameterSend,sizeof(CFG_INIT_INFO));
	//vos_EdrTaskDelay(TASK_DELAY_TIME);
	while(retValue != OK)
	{
		retValue = cfgResponseRecv((UINT8)com);
		num++;
		if(num == 0xFFFFFFFF)
		{
			printf("com :%d,ininit wait %d times\n\r", com, num);
		}
     }

	printf("com:%d, num is :%d\n\r", com, num);
	
	
	if(retValue == OK )
	{
		return OK;
	}
	
	return ERROR;	
	
}






/*com 1 4: CANfd  ;  2 3 CAN*/

INT32 comCanCfgInit(UINT32 com,UINT32 mode,UINT32 dataBpsRate)
{
	CFG_INIT_INFO cfgParameterSend ={0};
	INT32 retValue = 0; 


	cfgParameterSend.msgType = 0x11223344;
	cfgParameterSend.cfgRequestFlag = CFG_REQUEST_HEAD;
	cfgParameterSend.portNum = 1;
	
	
	//cfgParameterSend.mode = CAN_MODE;/*1 can mode ;2 canfd mode*/
        cfgParameterSend.mode = (UINT8)mode;/*1 can mode ;2 canfd mode*/
	cfgParameterSend.portId = (UINT8)com; /*1 2 3 4 */
	cfgParameterSend.bpsRate = dataBpsRate;
	cfgParameterSend.canFdRate = 0;
	
	
	appCanDataSendFunc(com,(char *)&cfgParameterSend,sizeof(CFG_INIT_INFO));
	vos_EdrTaskDelay(TASK_DELAY_TIME);
	retValue = cfgResponseRecv((UINT8)com);
	
	if(retValue == OK )
	{
		return OK;
	}
	
	return ERROR;

}




INT32 comCanFdCfgInit(UINT32 com,UINT32 mode,UINT32 dataBpsRate,UINT32 canFdBpsRate)
{



	CFG_INIT_INFO cfgParameterSend ={0};
	INT32 retValue = 0; 
	
	cfgParameterSend.msgType = 0x11223344;
	cfgParameterSend.cfgRequestFlag = CFG_REQUEST_HEAD;
	cfgParameterSend.portNum = 1;
	
	
	//cfgParameterSend.mode = CANFD_MODE;/*1 can mode ;2 canfd mode*/
        cfgParameterSend.mode = (UINT8)mode;
	cfgParameterSend.portId = (UINT8)com; /*1 2 3 4 */
	cfgParameterSend.bpsRate = dataBpsRate;
	cfgParameterSend.canFdRate = canFdBpsRate;
	
	
	appCanDataSendFunc(com,(char *)&cfgParameterSend,sizeof(CFG_INIT_INFO));
	vos_EdrTaskDelay(TASK_DELAY_TIME);
	retValue = cfgResponseRecv((UINT8)com);
	
	if(retValue == OK )
	{
		return OK;
	}
	
	return ERROR;

}


#if 10

void canProcessDataSendLifeAdd(void)
{
	
	UINT8 i = 0;
	
	g_sendDataLife++;
	
	for(i=0;i<64;i++)
	{
		g_array[i] = g_sendDataLife;
	}
	
}

#endif



/*just test  1 8     3 64*/
STATUS appCanDataSend(UINT32 com,UINT32 canId,UINT8 *dataPointer,UINT32 comDataLength)
{
	CAN_PROCESS_DATA  channel = {0};
	INT8 retValue = 0;

	UINT32 sendHead = 0;


        if(comDataLength > 8)
	{

            printf("data length is over 8 \n");
            return ERROR;

	}

        if((com > 8) ||(com < 1))
	{
            printf("com value is error \r\n");
            return ERROR;

	}
	
	channel.processType = 0x33445566;
	channel.processData.dataHead = 0x01;
	channel.processData.portId = (UINT8)com;
    //printf("send com is %d \r\n",com);
	channel.processData.canId = canId; /*canId*/
	channel.processData.frameDataLength = (UINT8)comDataLength; 
	channel.processData.reserver = 0x0;/*0 standard ID ;1 extended ID*/ 
        channel.processData.reserver1 = 0x0;
	memcpy(channel.processData.dataArray,dataPointer,comDataLength);
	//printf("sizeof(CAN_PROCESS_DATA): %d\n",sizeof(CAN_PROCESS_DATA));
	retValue = appCanDataSendFunc(com,(char *)&channel,sizeof(CAN_PROCESS_DATA));

	if(retValue == 0)
	{
		return OK;
	}
	else
	{
		return ERROR;
	}
}





STATUS compareSendAndRecvData(char *pointer,UINT8 dataLength)
{
	UINT8 retValue = 0;
	UINT8 i=0;
	
	retValue = memcmp((UINT8 *)(pointer+CAN_DATA_OFFSET),g_array,dataLength);
	
	if(retValue == 0)
	{
		return OK;
	}
         
	return ERROR;
	
}






STATUS compareCanSendAndRecvData(char *pointer,UINT8 dataLength)
{
	UINT8 retValue = 0;
	UINT8 i=0;
	
	retValue = memcmp((UINT8 *)(pointer+CAN_DATA_OFFSET),g_array,dataLength);
	
	if(retValue == 0)
	{
		return OK;
	}
         
	return ERROR;
	
}




#if 0


STATUS canAndCanFdTest_before(void)
{
	UINT8 i = 0;
	int retValue = 0;
	
	UINT8 configSucessFlag = 0;
	UINT8 configChannelOneSucessFlag = 0;
	UINT8 configChannelTwoSucessFlag = 0;
	UINT8 configChannelThreeSucessFlag = 0;
	UINT8 configChannelFourSucessFlag = 0;
	
	UINT32 *send1Pointer = NULL;
	UINT32 *send2Pointer = NULL;
	UINT32 *send3Pointer = NULL;
	UINT32 *send4Pointer = NULL;

	UINT32 *recv1Pointer = NULL;
	UINT32 *recv2Pointer = NULL;
	UINT32 *recv3Pointer = NULL;
	UINT32 *recv4Pointer = NULL;


	UINT32 send1 = 0;
	UINT32 send2 = 0;
	UINT32 send3= 0;
	UINT32 send4= 0;

	UINT32 recv1 = 0;
	UINT32 recv2 = 0;
	UINT32 recv3 = 0;
	UINT32 recv4 = 0;

	send1Pointer = &send1;
	send2Pointer = &send2;
	send3Pointer = &send3;
	send4Pointer = &send4;

	recv1Pointer = &recv1;
	recv2Pointer = &recv2;
	recv3Pointer = &recv3;
	recv4Pointer = &recv4;
 
	
	
	UINT8 dataBuffer[1432] = {0};
	
	char *dataPointer = NULL;
	
	dataPointer = (char *)dataBuffer;
	
	
	for(i=0;i<100;i++)
	{
		
		if(configChannelOneSucessFlag != CHANNEL_CONFG_SUCESS)
		{
			retValue = comCanFdCfgInit(CHANNEL_ONE,CANFD_MODE_STD,500,2000);
	        if(retValue == OK)
	        {
	        	configChannelOneSucessFlag = CHANNEL_CONFG_SUCESS;
	        	printf("config channel 1 sucess \n\r");
	        }
			
		}
#if 0   //**debug test
		if(configChannelTwoSucessFlag != CHANNEL_CONFG_SUCESS)
		{
			retValue = comCanCfgInit(CHANNEL_TWO,CAN_MODE_EXT,250);
	        if(retValue == OK)
	        {
	        	configChannelTwoSucessFlag = CHANNEL_CONFG_SUCESS;
	        	printf("config channel 2 sucess \n\r");
	        }
			
		}

		if(configChannelThreeSucessFlag != CHANNEL_CONFG_SUCESS)
		{
			
			retValue = comCanCfgInit(CHANNEL_THREE,CAN_MODE_EXT,250);
	        if(retValue == OK)
	        {
	        	configChannelThreeSucessFlag = CHANNEL_CONFG_SUCESS;
	        	printf("config channel 3 sucess \n\r");
	        }

		}

#endif
#if 10
		if(configChannelTwoSucessFlag != CHANNEL_CONFG_SUCESS)
		{
			retValue = comCanCfgInit(CHANNEL_TWO,CAN_MODE_STD,500);
	        if(retValue == OK)
	        {
	        	configChannelTwoSucessFlag = CHANNEL_CONFG_SUCESS;
	        	printf("config channel 2 sucess \n\r");
	        }
			
		}

		if(configChannelThreeSucessFlag != CHANNEL_CONFG_SUCESS)
		{
			
			retValue = comCanCfgInit(CHANNEL_THREE,CAN_MODE_STD,500);
	        if(retValue == OK)
	        {
	        	configChannelThreeSucessFlag = CHANNEL_CONFG_SUCESS;
	        	printf("config channel 3 sucess \n\r");
	        }

		}

#endif
		if(configChannelFourSucessFlag != CHANNEL_CONFG_SUCESS)
		{
			retValue = comCanFdCfgInit(CHANNEL_FOUR,CANFD_MODE_STD,500,2000);
	        if(retValue == OK)
	        {
	        	configChannelFourSucessFlag = CHANNEL_CONFG_SUCESS;
	        	printf("config channel 4 sucess \n\r");
	        }
			
		}
        
		printf("send times is %d \n",(i+1));

		if( (configChannelOneSucessFlag == CHANNEL_CONFG_SUCESS)&&(configChannelTwoSucessFlag == CHANNEL_CONFG_SUCESS) )
		{
			if((configChannelThreeSucessFlag == CHANNEL_CONFG_SUCESS)&&(configChannelFourSucessFlag == CHANNEL_CONFG_SUCESS))
			{
				
				configSucessFlag = CHANNEL_CONFG_SUCESS;
				printf("all channel config sucess \n\r ");
				break;
				
			}
			
		}
     
	}
	
	if(configSucessFlag == CHANNEL_CONFG_SUCESS)
	{
#if 10		
		//while(1)
		for(i=0;i<5;i++)
		{
			/*channel 1*/
					
					
					
#if 10			
                                        memset(g_array,0x0,sizeof(g_array));
					canProcessDataSendLifeAdd();
					canProcessDataSend(CHANNEL_ONE,0x1ff,g_array,CHANNEL_CAN_DATA_LENGTH);
					
					vos_EdrTaskDelay(TASK_DELAY_TIME);
					
					appCanFd2DataRecv(dataPointer,sizeof(CAN_PROCESS_DATA));
					
					retValue = compareSendAndRecvData(dataPointer,CHANNEL_CAN_DATA_LENGTH);
					
					if(retValue == OK)
					{
						g_channelOneTestSucess++;
					}
					else
					{
						g_channelOneTestError++;
					}
					memset(g_array,0x0,sizeof(g_array));
#endif

			/*channel 2*/
#if 10				

					vos_EdrTaskDelay(TASK_DELAY_TIME);
					canProcessDataSendLifeAdd();
					canProcessDataSend(CHANNEL_TWO,0x2ff,g_array,CHANNEL_CAN_DATA_LENGTH);
					
					vos_EdrTaskDelay(TASK_DELAY_TIME);
					
					appCanDataRecv(dataPointer,sizeof(CAN_PROCESS_DATA));
					
					retValue = compareSendAndRecvData(dataPointer,CHANNEL_CAN_DATA_LENGTH);
					
					if(retValue == OK)
					{
						g_channelTwoTestSucess++;
					}
					else
					{
						g_channelTwoTestError++;
					}
					memset(g_array,0x0,sizeof(g_array));	
					
#endif
			
			         /*channel 3*/
#if 10
					

					vos_EdrTaskDelay(TASK_DELAY_TIME);
					canProcessDataSendLifeAdd();
					canProcessDataSend(CHANNEL_THREE,0x3ff,g_array,CHANNEL_CAN_DATA_LENGTH);
					
					vos_EdrTaskDelay(TASK_DELAY_TIME);
					
					appCanDataRecv(dataPointer,sizeof(CAN_PROCESS_DATA));
					
					retValue = compareSendAndRecvData(dataPointer,CHANNEL_CAN_DATA_LENGTH);
					
					if(retValue == OK)
					{
						g_channelThreeTestSucess++;
					}
					else
					{
						g_channelThreeTestError++;
					}
					memset(g_array,0x0,sizeof(g_array));			
#endif					
			               /*channel 4*/
#if 10					

					vos_EdrTaskDelay(TASK_DELAY_TIME);
					canProcessDataSendLifeAdd();
					canProcessDataSend(CHANNEL_FOUR,0x4ff,g_array,CHANNEL_CAN_DATA_LENGTH);
					
					vos_EdrTaskDelay(TASK_DELAY_TIME);
					
					appCanFd1DataRecv(dataPointer,sizeof(CAN_PROCESS_DATA));
					
					retValue = compareSendAndRecvData(dataPointer,CHANNEL_CAN_DATA_LENGTH);
					
					if(retValue == OK)
					{
						g_channelFourTestSucess++;
					}
					else
					{
						g_channelFourTestError++;
					}
					
					memset(g_array,0x0,sizeof(g_array));	
#endif                                  vos_EdrTaskDelay(TASK_DELAY_TIME);

		                  //  printf("i :%d \n",i);	

		}

#if 10		
		getUdpSendStatistic(send1Pointer,send2Pointer,send3Pointer,send4Pointer);
		printf("recv all right num  is %d \n",*send1Pointer);
		printf("recv all error num  is %d \n",*send2Pointer);
		printf("send all right num  is %d \n",*send3Pointer);
		printf("send all error num  is %d \n",*send4Pointer);
		
		getUdpRecvStatistic(recv1Pointer,recv2Pointer,recv3Pointer,recv4Pointer);
		printf("recv can all right num  is %d \n",*recv1Pointer);
		printf("recv canfd1 all right num  is %d \n",*recv2Pointer);
		printf("recv canfd2 all right num  is %d \n",*recv3Pointer);
		printf("recv canfd3 all right num  is %d \n",*recv4Pointer);
#endif	
#if 0
		// printf("ok i :%d \n",i);	
		printf("g_channelOneTestError is %d \n",g_channelOneTestError);
		printf("g_channelTwoTestError is %d \n",g_channelTwoTestError);
		printf("g_channelThreeTestError is %d \n",g_channelThreeTestError);
		printf("g_channelFourTestError is %d \n",g_channelFourTestError);
#endif		

		if((g_channelOneTestError == 0) && (g_channelTwoTestError == 0) )
		{
			if((g_channelThreeTestError == 0) &&(g_channelFourTestError == 0))
			{
				return OK;
			}
		}
		
		return ERROR;
#endif	
	}
	else
	{

	     printf("config channel failure \r \n");
	     return ERROR;
             
	}
	
}


#endif


// Function to initialize CAN standard mode configuration  
int comCanSTDCfgInit(int channel_id,int buad_rate)  
{  
    // Initialize CAN configuration for the given channel in standard mode with a baud rate of 500kbps  
    int retValue = comCfgInit(channel_id, CAN_MODE_STD, buad_rate);  
  
    // Check if the configuration was successful  
    if (retValue == OK)  
    {  
        // If successful, print a success message  
        printf("Configuration of channel %d successful\n\r", channel_id);  
        return OK; // Return success  
    }  
    else  
    {  
        // If not successful, print an error message  
        printf("Configuration of channel %d failed\n\r", channel_id);  
        return ERROR; // Return error  
    }  
}  

INT32 TASK_DELAY()
{
	vos_EdrTaskDelay(TASK_DELAY_TIME);
}




STATUS canAndCanFdTest(void)
{
	UINT8 i = 0;
	int retValue = 0;
	
	UINT8 configSucessFlag = 0;
	UINT8 configChannelOneSucessFlag = 0;
	UINT8 configChannelTwoSucessFlag = 0;
	UINT8 configChannelThreeSucessFlag = 0;
	UINT8 configChannelFourSucessFlag = 0;
	
	UINT32 *send1Pointer = NULL;
	UINT32 *send2Pointer = NULL;
	UINT32 *send3Pointer = NULL;
	UINT32 *send4Pointer = NULL;

	UINT32 *recv1Pointer = NULL;
	UINT32 *recv2Pointer = NULL;
	UINT32 *recv3Pointer = NULL;
	UINT32 *recv4Pointer = NULL;
	UINT32 *recv5Pointer = NULL;
	UINT32 *recv6Pointer = NULL;
	UINT32 *recv7Pointer = NULL;
	UINT32 *recv8Pointer = NULL;


	UINT32 send1 = 0;
	UINT32 send2 = 0;
	UINT32 send3= 0;
	UINT32 send4= 0;

	UINT32 recv1 = 0;
	UINT32 recv2 = 0;
	UINT32 recv3 = 0;
	UINT32 recv4 = 0;

	send1Pointer = &send1;
	send2Pointer = &send2;
	send3Pointer = &send3;
	send4Pointer = &send4;

	recv1Pointer = &recv1;
	recv2Pointer = &recv2;
	recv3Pointer = &recv3;
	recv4Pointer = &recv4;
 
	
	
	UINT8 dataBuffer[1432] = {0};
	
	char *dataPointer= NULL;
	
	dataPointer = (char *)dataBuffer;
	
	
	for(i=0;i<100;i++)
	{
		
		if(configChannelOneSucessFlag != CHANNEL_CONFG_SUCESS)
		{
			retValue = comCfgInit(CHANNEL_ONE,CAN_MODE_STD,500);
	        if(retValue == OK)
	        {
	        	configChannelOneSucessFlag = CHANNEL_CONFG_SUCESS;
	        	printf("config channel 1 sucess \n\r");
	        }
			
		}

#if 10
		if(configChannelTwoSucessFlag != CHANNEL_CONFG_SUCESS)
		{
			retValue = comCfgInit(CHANNEL_TWO,CAN_MODE_STD,500);
	        if(retValue == OK)
	        {
	        	configChannelTwoSucessFlag = CHANNEL_CONFG_SUCESS;
	        	printf("config channel 2 sucess \n\r");
	        }
			
		}

		if(configChannelThreeSucessFlag != CHANNEL_CONFG_SUCESS)
		{
			
			retValue = comCfgInit(CHANNEL_THREE,CAN_MODE_STD,500);
	        if(retValue == OK)
	        {
	        	configChannelThreeSucessFlag = CHANNEL_CONFG_SUCESS;
	        	printf("config channel 3 sucess \n\r");
	        }

		}

#endif
		if(configChannelFourSucessFlag != CHANNEL_CONFG_SUCESS)
		{
			retValue = comCfgInit(CHANNEL_FOUR,CAN_MODE_STD,500);
	        if(retValue == OK)
	        {
	        	configChannelFourSucessFlag = CHANNEL_CONFG_SUCESS;
	        	printf("config channel 4 sucess \n\r");
	        }
			
		}
        
		printf("send times is %d \n",(i+1));

		if( (configChannelOneSucessFlag == CHANNEL_CONFG_SUCESS)&&(configChannelTwoSucessFlag == CHANNEL_CONFG_SUCESS) )
		{
			if((configChannelThreeSucessFlag == CHANNEL_CONFG_SUCESS)&&(configChannelFourSucessFlag == CHANNEL_CONFG_SUCESS))
			{
				
				configSucessFlag = CHANNEL_CONFG_SUCESS;
				printf("all channel config sucess \n\r ");
				break;
				
			}
			
		}
     
	}

#if 1
	
	if(configSucessFlag == CHANNEL_CONFG_SUCESS)
	{
#if 10		
		//while(1)
		for(i=0;i<5;i++)
		{
			/*channel 1*/
					
					
					
#if 0			
                    memset(g_array,0x0,sizeof(g_array));
					canProcessDataSendLifeAdd();
					appCanDataSend(CHANNEL_ONE,0x1ff,g_array,CHANNEL_CAN_DATA_LENGTH);
					
					vos_EdrTaskDelay(TASK_DELAY_TIME);
					
					appCanDataRecv(4,dataPointer,sizeof(CAN_PROCESS_DATA));
					
					retValue = compareSendAndRecvData(dataPointer,CHANNEL_CAN_DATA_LENGTH);
					
					if(retValue == OK)
					{
						g_channelOneTestSucess++;
					}
					else
					{
						g_channelOneTestError++;
					}
					memset(g_array,0x0,sizeof(g_array));
#endif

			/*channel 2*/
#if 0				

					vos_EdrTaskDelay(TASK_DELAY_TIME);
					canProcessDataSendLifeAdd();
					appCanDataSend(3,0x2ff,g_array,CHANNEL_CAN_DATA_LENGTH);
					
					vos_EdrTaskDelay(TASK_DELAY_TIME);
					
					appCanDataRecv(2,dataPointer,sizeof(CAN_PROCESS_DATA));
					
					retValue = compareCanSendAndRecvData(dataPointer,CHANNEL_CAN_DATA_LENGTH);
					
					if(retValue == OK)
					{
						g_channelTwoTestSucess++;
					}
					else
					{
						g_channelTwoTestError++;
					}
					memset(g_array,0x0,sizeof(g_array));	
					
#endif
			
			         /*channel 3*/
#if 0
					

					vos_EdrTaskDelay(TASK_DELAY_TIME);
					canProcessDataSendLifeAdd();
					appCanDataSend(2,0x3ff,g_array,CHANNEL_CAN_DATA_LENGTH);
					
					vos_EdrTaskDelay(TASK_DELAY_TIME);
					
					appCanDataRecv(3,dataPointer,sizeof(CAN_PROCESS_DATA));
					
					retValue = compareCanSendAndRecvData(dataPointer,CHANNEL_CAN_DATA_LENGTH);
					
					if(retValue == OK)
					{
						g_channelThreeTestSucess++;
					}
					else
					{
						g_channelThreeTestError++;
					}
					memset(g_array,0x0,sizeof(g_array));			
#endif					
			               /*channel 4*/
#if 0					

					vos_EdrTaskDelay(TASK_DELAY_TIME);
					canProcessDataSendLifeAdd();
					appCanDataSend(4,0x4ff,g_array,CHANNEL_CAN_DATA_LENGTH);
					
					vos_EdrTaskDelay(TASK_DELAY_TIME);
					
					appCanDataRecv(1,dataPointer,sizeof(CAN_PROCESS_DATA));
					
					retValue = compareSendAndRecvData(dataPointer,CHANNEL_CAN_DATA_LENGTH);
					
					if(retValue == OK)
					{
						g_channelFourTestSucess++;
					}
					else
					{
						g_channelFourTestError++;
					}
					
					memset(g_array,0x0,sizeof(g_array));	
#endif                                  vos_EdrTaskDelay(TASK_DELAY_TIME);

		                  //  printf("i :%d \n",i);	

		}
                
                printf("g_channelOneTestSucess is %d,error is %d  \r\n",g_channelOneTestSucess,g_channelOneTestError);
                printf("g_channelTwoTestSucess is %d,error is %d \r\n",g_channelTwoTestSucess,g_channelTwoTestError);
                printf("g_channelThreeTestSucess is %d,error is %d \r\n",g_channelThreeTestSucess,g_channelThreeTestError);
                printf("g_channelFourTestSucess is %d,error is %d \r\n",g_channelFourTestSucess,g_channelFourTestError);
                

#if 10		
		getUdpSendStatistic(send1Pointer,send2Pointer,send3Pointer,send4Pointer);
		printf("recv all right num  is %d \n",*send1Pointer);
		printf("recv all error num  is %d \n",*send2Pointer);
		printf("send all right num  is %d \n",*send3Pointer);
		printf("send all error num  is %d \n",*send4Pointer);
#endif		
		getUdpRecvStatistic(recv1Pointer,recv2Pointer,recv3Pointer,recv4Pointer,recv5Pointer,recv6Pointer,recv7Pointer,recv8Pointer);
		printf("recv can1 all right num  is %d \n",*recv1Pointer);
		printf("recv can2 all right num  is %d \n",*recv2Pointer);
		printf("recv can3 all right num  is %d \n",*recv3Pointer);
		printf("recv can4 all right num  is %d \n",*recv4Pointer);
		printf("recv can5 all right num  is %d \n",*recv5Pointer);
		printf("recv canfd1 all right num  is %d \n",*recv6Pointer);
		printf("recv canfd2 all right num  is %d \n",*recv7Pointer);
		printf("recv canfd3 all right num  is %d \n",*recv8Pointer);
	
#if 0
		// printf("ok i :%d \n",i);	
		printf("g_channelOneTestError is %d \n",g_channelOneTestError);
		printf("g_channelTwoTestError is %d \n",g_channelTwoTestError);
		printf("g_channelThreeTestError is %d \n",g_channelThreeTestError);
		printf("g_channelFourTestError is %d \n",g_channelFourTestError);
#endif		

		if((g_channelOneTestError == 0) && (g_channelTwoTestError == 0) )
		{
			if((g_channelThreeTestError == 0) &&(g_channelFourTestError == 0))
			{
				return OK;
			}
		}
		
		return ERROR;
#endif	
	}
	else
	{

	     printf("config channel failure \r \n");
	     return ERROR;
             
	}

#endif
	
}




#if 0


STATUS canAndCanFdDebugTest(void)
{
	UINT8 i = 0;
	int retValue = 0;

 
	
	
	UINT8 dataBuffer[1432] = {0};
	
	char *dataPointer = NULL;
	
	dataPointer = (char *)dataBuffer;
	

		
		//while(1)
		for(i=0;i<20;i++)
		{
			/*channel 1*/
					
					
					
#if 10			
                    
					canProcessDataSendLifeAdd();
					canProcessDataSend(CHANNEL_ONE,0x1ff,g_array,CHANNEL_CANFD_DATA_LENGTH);
					
					vos_EdrTaskDelay(TASK_DELAY_TIME);
					
					appCanFd3DataRecv(dataPointer,sizeof(CAN_PROCESS_DATA));
					
					retValue = compareSendAndRecvData(dataPointer,CHANNEL_CANFD_DATA_LENGTH);
					
					if(retValue == OK)
					{
						g_channelOneTestSucess++;
					}
					else
					{
						g_channelOneTestError++;
					}
					memset(g_array,0x0,sizeof(g_array));
#endif

			/*channel 2*/
#if 10				

					vos_EdrTaskDelay(TASK_DELAY_TIME);
					canProcessDataSendLifeAdd();
					canProcessDataSend(CHANNEL_SIX,0x2ff,g_array,CHANNEL_CAN_DATA_LENGTH);
					
					vos_EdrTaskDelay(TASK_DELAY_TIME);
					
					appCanDataRecv(dataPointer,sizeof(CAN_PROCESS_DATA));
					
					retValue = compareSendAndRecvData(dataPointer,CHANNEL_CAN_DATA_LENGTH);
					
					if(retValue == OK)
					{
						g_channelTwoTestSucess++;
					}
					else
					{
						g_channelTwoTestError++;
					}
					memset(g_array,0x0,sizeof(g_array));	
					
#endif
			
			         /*channel 3*/
#if 10
					

					vos_EdrTaskDelay(TASK_DELAY_TIME);
					canProcessDataSendLifeAdd();
					canProcessDataSend(CHANNEL_FIVE,0x3ff,g_array,CHANNEL_CAN_DATA_LENGTH);
					
					vos_EdrTaskDelay(TASK_DELAY_TIME);
					
					appCanDataRecv(dataPointer,sizeof(CAN_PROCESS_DATA));
					
					retValue = compareSendAndRecvData(dataPointer,CHANNEL_CAN_DATA_LENGTH);
					
					if(retValue == OK)
					{
						g_channelThreeTestSucess++;
					}
					else
					{
						g_channelThreeTestError++;
					}
					memset(g_array,0x0,sizeof(g_array));			
#endif					
			               /*channel 4*/
#if 10					

					vos_EdrTaskDelay(TASK_DELAY_TIME);
					canProcessDataSendLifeAdd();
					canProcessDataSend(CHANNEL_TWO,0x4ff,g_array,CHANNEL_CANFD_DATA_LENGTH);
					
					vos_EdrTaskDelay(TASK_DELAY_TIME);
					
					appCanFd1DataRecv(dataPointer,sizeof(CAN_PROCESS_DATA));
					
					retValue = compareSendAndRecvData(dataPointer,CHANNEL_CANFD_DATA_LENGTH);
					
					if(retValue == OK)
					{
						g_channelFourTestSucess++;
					}
					else
					{
						g_channelFourTestError++;
					}
					
					memset(g_array,0x0,sizeof(g_array));	
#endif

		                  //  printf("i :%d \n",i);	

		}	

		if((g_channelOneTestError == 0) && (g_channelTwoTestError == 0) )
		{
			if((g_channelThreeTestError == 0) &&(g_channelFourTestError == 0))
			{
				return OK;
			}
		}
		
		return ERROR;
	
}


#endif

/*\B7\A2\CB\CDUDP\CA\FD\BE\DD*/

void udp_sendTestMain(void)
{
	UINT8 sendDataArray[32] = {0};
	UINT8 i = 0;
	UINT8 sendTimes = 0 ;
	
	for(i=0;i<32;i++)
	{
		
		sendDataArray[i] = i;
	}
	
	
	while(1)
	{
		sendTimes++;
		sendDataArray[0] = sendTimes;	
		
		vos_EdrMsgQSend(g_RsDataToUdpMsgID,(char *)sendDataArray,sizeof(sendDataArray),0,MSG_PRI_NORMAL);
		
		vos_EdrTaskDelay(5000);	
	}	
	
	
}



STATUS udp_sendTestTask (void)
{
	STATUS st = ERROR;
	
	st = vos_EdrTaskSpawn ( "t_udpSendTest", 25, VOS_VX_FP_TASK, 409600, ( FUNCPTR ) udp_sendTestMain,( INT32 ) NULL );

	if ( st == ERROR )
	{
		printf ( "udp_send test task create failed !\n" );
 
	}

	return st;
}


STATUS canAndCanFdTestTask (void)
{
	STATUS st = ERROR;
	
	st = vos_EdrTaskSpawn ( "t_canAndCanFdTestTask", 29, VOS_VX_FP_TASK, 40960, ( FUNCPTR ) canAndCanFdTest,( INT32 ) NULL );

	if ( st == ERROR )
	{
		printf ( "t_canAndCanFdTestTask create failed !\n" );
 
	}

	return st;
}



