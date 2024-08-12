


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

extern void setUdpServerCan1CallBackFunc(udpServerCallBackFunc callBackFuc);
extern void setUdpServerCan2CallBackFunc(udpServerCallBackFunc callBackFuc);
extern void setUdpServerCan3CallBackFunc(udpServerCallBackFunc callBackFuc);
extern void setUdpServerCan4CallBackFunc(udpServerCallBackFunc callBackFuc);
extern void setUdpServerCan5CallBackFunc(udpServerCallBackFunc callBackFuc);




extern void setUdpServerCanFd1CallBackFunc(udpServerCallBackFunc callBackFuc);
extern void setUdpServerCanFd2CallBackFunc(udpServerCallBackFunc callBackFuc);
extern void setUdpServerCanFd3CallBackFunc(udpServerCallBackFunc callBackFuc);



extern UINT8 g_recvCanLifeArray[1432];	
extern UINT8 g_recvCanDataArray[1432];


extern UINT8 g_recvCan1DataArray[1432];	
extern UINT8 g_recvCan2DataArray[1432];	
extern UINT8 g_recvCan3DataArray[1432];	
extern UINT8 g_recvCan4DataArray[1432];	
extern UINT8 g_recvCan5DataArray[1432];	




extern UINT8 g_recvCanFd1DataArray[1432];	
extern UINT8 g_recvCanFd2DataArray[1432];	
extern UINT8 g_recvCanFd3DataArray[1432];


		
extern int plc_main(void);


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





#if 10


INT32 recvUdpCanLifeFunc(char *data,UINT32 dataLength)
{
	UINT16 i = 0;

	
	vos_EdrSemWait(&g_udpRecvLifeDataSem_ID, WAIT_FOREVER);
	memcpy(g_recvCanLifeArray,data,dataLength);
	vos_EdrSemPost(&g_udpRecvLifeDataSem_ID);
	
	
	return 0;
	
}






INT32 recvUdpCanDataFunc(char *data,UINT32 dataLength)
{
	UINT16 i = 0;

	
	vos_EdrSemWait(&g_udpRecvCanDataSem_ID, WAIT_FOREVER);
	memcpy(g_recvCanDataArray,data,dataLength);
	vos_EdrSemPost(&g_udpRecvCanDataSem_ID);
	
	
	return 0;
	
}






INT32 recvUdpCan1DataFunc(char *data,UINT32 dataLength)
{
	UINT16 i = 0;

	
	vos_EdrSemWait(&g_udpRecvCan1DataSem_ID, WAIT_FOREVER);
	memcpy(g_recvCan1DataArray,data,dataLength);
/*printf("can1 recv data is : \r\n");
for(i=0;i<dataLength;i++)
{

printf("%x ",g_recvCan1DataArray[i]);

}
printf("\r\n");*/
	vos_EdrSemPost(&g_udpRecvCan1DataSem_ID);
	
	
	return 0;
	
}

INT32 recvUdpCan2DataFunc(char *data,UINT32 dataLength)
{
	UINT16 i = 0;

	
	vos_EdrSemWait(&g_udpRecvCan2DataSem_ID, WAIT_FOREVER);
	memcpy(g_recvCan2DataArray,data,dataLength);
/*printf("can2 recv data is : \r\n");
for(i=0;i<dataLength;i++)
{

printf("%x ",g_recvCan2DataArray[i]);

}
printf("\r\n");*/
	vos_EdrSemPost(&g_udpRecvCan2DataSem_ID);
	
	
	return 0;
	
}


INT32 recvUdpCan3DataFunc(char *data,UINT32 dataLength)
{
	UINT16 i = 0;

	
	vos_EdrSemWait(&g_udpRecvCan3DataSem_ID, WAIT_FOREVER);
	memcpy(g_recvCan3DataArray,data,dataLength);
/*printf("can3 recv data is : \r\n");
for(i=0;i<dataLength;i++)
{

printf("%x ",g_recvCan3DataArray[i]);

}
printf("\r\n");*/
	vos_EdrSemPost(&g_udpRecvCan3DataSem_ID);
	
	
	return 0;
	
}

INT32 recvUdpCan4DataFunc(char *data,UINT32 dataLength)
{
	UINT16 i = 0;

	
	vos_EdrSemWait(&g_udpRecvCan4DataSem_ID, WAIT_FOREVER);
	memcpy(g_recvCan4DataArray,data,dataLength);
/*printf("can4 recv data is : \r\n");
for(i=0;i<dataLength;i++)
{

printf("%x ",g_recvCan4DataArray[i]);

}
printf("\r\n");*/
	vos_EdrSemPost(&g_udpRecvCan4DataSem_ID);
	
	
	return 0;
	
}

INT32 recvUdpCan5DataFunc(char *data,UINT32 dataLength)
{
	UINT16 i = 0;

	
	vos_EdrSemWait(&g_udpRecvCan5DataSem_ID, WAIT_FOREVER);
	memcpy(g_recvCan5DataArray,data,dataLength);
/*printf("can5 recv data is : \r\n");
for(i=0;i<dataLength;i++)
{

printf("%x ",g_recvCan5DataArray[i]);

}
printf("\r\n");*/
	vos_EdrSemPost(&g_udpRecvCan5DataSem_ID);
	
	
	return 0;
	
}






INT32 recvUdpCanFd1DataFunc(char *data,UINT32 dataLength)
{
	UINT16 i = 0;

	
	vos_EdrSemWait(&g_udpRecvCanFd1DataSem_ID, WAIT_FOREVER);
	memcpy(g_recvCanFd1DataArray,data,dataLength);
/*printf("canfd1 recv data is : \r\n");
for(i=0;i<dataLength;i++)
{

printf("%x ",g_recvCanFd1DataArray[i]);

}
printf("\r\n");*/
	vos_EdrSemPost(&g_udpRecvCanFd1DataSem_ID);
	
	
	return 0;
	

	
}


INT32 recvUdpCanFd2DataFunc(char *data,UINT32 dataLength)
{
	UINT16 i = 0;

	
	vos_EdrSemWait(&g_udpRecvCanFd2DataSem_ID, WAIT_FOREVER);
	memcpy(g_recvCanFd2DataArray,data,dataLength);
/*printf("canfd2 recv data is : \r\n");
for(i=0;i<dataLength;i++)
{

printf("%x ",g_recvCanFd2DataArray[i]);

}
printf("\r\n");*/
	vos_EdrSemPost(&g_udpRecvCanFd2DataSem_ID);
	
	
	return 0;
	

	
}


INT32 recvUdpCanFd3DataFunc(char *data,UINT32 dataLength)
{
	UINT16 i = 0;
	
	vos_EdrSemWait(&g_udpRecvCanFd3DataSem_ID, WAIT_FOREVER);
	memcpy(g_recvCanFd3DataArray,data,dataLength);
/*printf("canfd3 recv data is : \r\n");
for(i=0;i<dataLength;i++)
{

printf("%x ",g_recvCanFd3DataArray[i]);

}
printf("\r\n");*/
	vos_EdrSemPost(&g_udpRecvCanFd3DataSem_ID);
	
	return 0;
	

	
}






INT32  udpCanStart(void)
{
	
#if 0	
	//gpioInit();
	
        //plat_uartAllInit();

    udp_Main();
	
	setUdpServerCan1CallBackFunc(recvUdpCan1DataFunc);
	setUdpServerCan2CallBackFunc(recvUdpCan2DataFunc);
	setUdpServerCan3CallBackFunc(recvUdpCan3DataFunc);
	setUdpServerCan4CallBackFunc(recvUdpCan4DataFunc);
	setUdpServerCan5CallBackFunc(recvUdpCan5DataFunc);
	
	setUdpServerCanFd1CallBackFunc(recvUdpCanFd1DataFunc);
	setUdpServerCanFd2CallBackFunc(recvUdpCanFd2DataFunc);
	setUdpServerCanFd3CallBackFunc(recvUdpCanFd3DataFunc);
#endif


	
	//canAndCanFdTestTask();
	
	
	//ledSysRunTask();
	
	//trdp_Main();
	
	//trdp_test();

    //plc_main();
      return 0;


}

#endif
