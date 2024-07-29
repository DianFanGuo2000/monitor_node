


#include "stdio.h"


#include "vos_EdrMessage.h"
#include "mng_Interface.h"
#include "vos_EdrTask.h"

#include "vos_EdrSemaphore.h"


UINT32 g_recvAllPackets = 0;
UINT32 g_recvAllBytes = 0;

extern void udp_Main(void);
extern void trdp_Main(void);
extern STATUS vos_EdrTaskDelay(INT32);

extern STATUS ledSysRunTask (void);

extern STATUS canAndCanFdTestTask (void);


extern void trdp_test(void);

extern void gpioInit(void);

extern void plat_uartAllInit(void);

typedef  INT32 (*udpServerCallBackFunc)(char *data,UINT32 dataLength);

extern void setUdpServerCallBackFunc(udpServerCallBackFunc callBackFuc);
extern void setUdpServerCanFd1CallBackFunc(udpServerCallBackFunc callBackFuc);
extern void setUdpServerCanFd2CallBackFunc(udpServerCallBackFunc callBackFuc);
extern void setUdpServerCanFd3CallBackFunc(udpServerCallBackFunc callBackFuc);


extern UINT8 g_recvDataArray[1432];	
extern UINT8 g_recvCanFd1DataArray[1432];	
extern UINT8 g_recvCanFd2DataArray[1432];	
extern UINT8 g_recvCanFd3DataArray[1432];


		
extern int plc_main(void);

extern VOS_EDR_SEM_ID g_udpRecvDataSem_ID;
extern VOS_EDR_SEM_ID g_udpRecvCanFd1DataSem_ID;
extern VOS_EDR_SEM_ID g_udpRecvCanFd2DataSem_ID;
extern VOS_EDR_SEM_ID g_udpRecvCanFd3DataSem_ID;





#if 10

INT32 recvUdpDataFunc(char *data,UINT32 dataLength)
{
	UINT16 i = 0;


      //printf("ok ok \n");
	//g_recvAllPackets++;
#if 0
       g_recvAllBytes = g_recvAllBytes + data[5];
       
        printf("packets all is %d \n",g_recvAllBytes);
	
	for(i=0;i<dataLength;i++)
	{
		
		printf("%x ",data[i]);
	}
        printf("\n");
#endif
	
	vos_EdrSemWait(&g_udpRecvDataSem_ID, WAIT_FOREVER);
	memcpy(g_recvDataArray,data,dataLength);
	vos_EdrSemPost(&g_udpRecvDataSem_ID);
	
	//printf("\n");
	
	return 0;
	

	
}


INT32 recvUdpCanFd1DataFunc(char *data,UINT32 dataLength)
{
	UINT16 i = 0;

	
#if 0	
	for(i=0;i<dataLength;i++)
	{
		
		printf("%x ",data[i]);
	}

#endif
	
	vos_EdrSemWait(&g_udpRecvCanFd1DataSem_ID, WAIT_FOREVER);
	memcpy(g_recvCanFd1DataArray,data,dataLength);
	vos_EdrSemPost(&g_udpRecvCanFd1DataSem_ID);
	
	//printf("\n");
	
	return 0;
	

	
}


INT32 recvUdpCanFd2DataFunc(char *data,UINT32 dataLength)
{
	UINT16 i = 0;

	
#if 0	

	for(i=0;i<dataLength;i++)
	{
		
		printf("%x ",data[i]);
	}

#endif
	
	vos_EdrSemWait(&g_udpRecvCanFd2DataSem_ID, WAIT_FOREVER);
	memcpy(g_recvCanFd2DataArray,data,dataLength);
	vos_EdrSemPost(&g_udpRecvCanFd2DataSem_ID);
	
	//printf("\n");
	
	return 0;
	

	
}


INT32 recvUdpCanFd3DataFunc(char *data,UINT32 dataLength)
{
	UINT16 i = 0;

	
#if 0	
	for(i=0;i<dataLength;i++)
	{
		
		printf("%x ",data[i]);
	}

#endif
	
	vos_EdrSemWait(&g_udpRecvCanFd3DataSem_ID, WAIT_FOREVER);
	memcpy(g_recvCanFd3DataArray,data,dataLength);
	vos_EdrSemPost(&g_udpRecvCanFd3DataSem_ID);
	
	//printf("\n");
	
	return 0;
	

	
}






INT32  udpCanStart(void)
{
	
	
	//gpioInit();
	
        //plat_uartAllInit();

        udp_Main();
	
	setUdpServerCallBackFunc(recvUdpDataFunc);
	setUdpServerCanFd1CallBackFunc(recvUdpCanFd1DataFunc);
	setUdpServerCanFd2CallBackFunc(recvUdpCanFd2DataFunc);
	setUdpServerCanFd3CallBackFunc(recvUdpCanFd3DataFunc);
	
	//canAndCanFdTestTask();
	
	
	//ledSysRunTask();
	
	//trdp_Main();
	
	//trdp_test();

        //plc_main();

	
       return 0;
}

#endif
