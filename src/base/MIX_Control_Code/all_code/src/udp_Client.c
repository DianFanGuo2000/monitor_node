/******************************************************************************
 *File Name    :    udp_Client.c
 *Copyright    :    Zhuzhou CSR Times Electric Co.,Ltd. All Rights Reserved.
 *Create Date  :    2012/03/15
 *Description  :    UDP Client driver
 *
 *REV1.0.0    Moyun   2012/03/15  File Create
 *
******************************************************************************/
/******************************************************************************
 *    Include File Section
 ******************************************************************************/


#include <sys/socket.h>
//#include <netinet/ether.h>
#include <stdio.h>
#include <netinet/in.h>


#include "string.h"

#include "udp_Server.h"
#include "udp_Client.h"

#include "vos_EdrMessage.h"
#include "mng_Interface.h"
#include "vos_EdrTask.h"

#include <string.h>

/******************************************************************************
 *    Debug switch Section
 ******************************************************************************/
/*#define UDP_CLIENT_DEBUG*/
/*#undef  UDP_CLIENT_DEBUG*/
/******************************************************************************
 *    Global Variable Define Section
 ******************************************************************************/

/******************************************************************************
 *    Local Macro Define Section
 ******************************************************************************/



VOS_EDR_MSG_Q_ID     g_RsDataToUdpMsgID;

//VOS_EDR_MSG_Q_ID     g_udpServerSendMsgID;

#define  UDP_MAX_SEND_LEN   (1432)



struct RS_DATA_SAVE_FRAME_MSG
{


    UINT8  dataArray[200];

};




UDP_SEND_STATISTIC g_udpSendCount = {0};



/******************************************************************************
 *    Local Struct  Define Section
 ******************************************************************************/

/******************************************************************************
 *    Local Prototype Declare Section
 ******************************************************************************/


/******************************************************************************
 *    Static Variable Define Section
 ******************************************************************************/

/******************************************************************************
 *    Function Define Section
 ******************************************************************************/

/******************************************************************************
 * Name:  UINT32 RsEvtMsgInit( void )
 * Function: init the event msg
 * Input:  non
 * return : non
 * Create by ningqiao  2012-5-16
 *******************************************************************************/
UINT32 UdpDataSendMsgInit ( void )
{
	STATUS st = ERROR;
	
       // printf("udp send msg create \n");
	st = vos_EdrMsgQCreate ( &g_RsDataToUdpMsgID, RSEVTMSGCNTMAX, 1432, VOS_MSG_Q_FIFO, IPC_CREAT | 0660 );

	/*create the msg for send to list*/
	if ( ERROR == st )
	{
		( void ) printf ( "g_RsDataToUdpMsgID create error!\n" );
		return 1U;
	}
	else
	{
	    ;//printf("udp send msg create ok \n");
	}

	return 0U;
}


/******************************************************************************
 * Name: STATUS udp_ClientCreateMain(void)
 * Function: the client data send data function.
 * Information: send the client data
 * Input: non.
 * return : OK,if the data send successed,otherwise return ERROR.
 * Create by moyun  2012-03-15
 ******************************************************************************/
STATUS udp_ClientProcessSendMain ( void )
{
	
	struct sockaddr_in ServerAddr_ST;       /* server's socket address */
	struct sockaddr_in ClientAddr_ST;
	INT32 SockAddrSize_I32 = 0;     /* size of socket address structure */
	INT32 SocketFD_I32 = 0;       /* socket file descriptor */
	INT32 nStatus = 0;
	UINT32 lFlagTrue = ( UINT32 ) ~0;
	int j = 0;
	int i = 0;
	UINT8 Mode_U8 = 0U;
	UINT32 Port_U32 = 0U;
	INT8 ServerIpAddr_pI8[INET_ADDR_LEN] = {0};
	INT8 HostIpAddr_pI8[INET_ADDR_LEN] = {0};
	UINT32 DelayTime_U32 = 0U;
	INT32 TheMutiTTLVal_I32 = UDP_MULTI_TTL_VAL;
	INT32 TheSingleTTLVal_I32 = UDP_MULTI_TTL_VAL;
	struct in_addr IfAddr_ST;
	struct sockaddr_in Sin_ST;
	INT32 recvDataLengthValue = 0;

	/*the udp data struct.*/
	struct RS_DATA_SAVE_FRAME_MSG   TempEvtMsg_ST;
	char   Data_aC8[UDP_MAX_SEND_LEN]    = { 0 };

	Port_U32 = UDP_SEND_PORT;
	
	//strncpy((char*)ServerIpAddr_pI8, (char*)IpAddr_aC8, INET_ADDR_LEN);

	if ( ( SocketFD_I32 = socket ( AF_INET, SOCK_DGRAM, 0 ) ) == ERROR )
	{
		printf ( "UDP socket creat error!\n\r" );

		return ( ERROR );
	}

	/* build server socket address */
	SockAddrSize_I32 = sizeof ( struct sockaddr_in );
	bzero ( ( char* ) &ClientAddr_ST, SockAddrSize_I32 );
	
	ClientAddr_ST.sin_family = AF_INET;
	ClientAddr_ST.sin_port = htons ( Port_U32 );
	
	//ClientAddr_ST.sin_addr.s_addr = inet_addr ( ( char* ) g_Rs422Tty33UdpData_ST.IpAddr_aC8 );
	ClientAddr_ST.sin_addr.s_addr = inet_addr((char *)(UDP_CLENT_TO_IP_VALUE));
	/*add ttl 2012-12-27*/
	if ( ( nStatus = setsockopt ( SocketFD_I32, IPPROTO_IP, IP_MULTICAST_TTL, ( char* ) &TheMutiTTLVal_I32,
	                              sizeof ( TheMutiTTLVal_I32 ) ) ) < 0 )
	{
		printf ( "setsockopt Set multi TTL error:\n" );
		close ( SocketFD_I32 );

		return ( ERROR );

	}

	/*add single ttl value, 2013-03-29*/
	if ( ( nStatus = setsockopt ( SocketFD_I32, IPPROTO_IP, IP_TTL, ( char* ) &TheSingleTTLVal_I32,
	                              sizeof ( TheSingleTTLVal_I32 ) ) ) < 0 )
	{
		printf ( "setsockopt Set single TTL error:\n" );
		close ( SocketFD_I32 );
		return ( ERROR );

	}


	while ( 1 )
	{

        memset((void *)Data_aC8,0x0,UDP_MAX_SEND_LEN);
		if ( ( recvDataLengthValue  = vos_EdrMsgQReceive ( g_RsDataToUdpMsgID, ( void* ) Data_aC8,
		                         UDP_MAX_SEND_LEN, VOS_MSG_WAIT_FOREVER ) ) != ERROR )

		{
			
			g_udpSendCount.udpQrecvRightNum++;
			
					if ( sendto ( SocketFD_I32,(char *)(Data_aC8+sizeof(long)),(recvDataLengthValue-sizeof(long)) , \
					              0, ( struct sockaddr* ) &ClientAddr_ST, SockAddrSize_I32 ) == ERROR )
					{
						
						g_udpSendCount.udpSendErrorNum++;
						
						close ( SocketFD_I32 );
						return ( ERROR );
					}
					else
					{
						
						g_udpSendCount.udpSendRightNum++;
#if 0
					//	if ( g_udpClientDebugMode_I32 == 1 )
						//{
							printf ( "\n\n" );
							
							for ( i = 0; i < ( INT32 ) (recvDataLengthValue-sizeof(long)); i++ )
							{
								printf ( "%02x ", Data_aC8[i+8] );

								if ( ( i + 1 ) % 16 == 0 )
								{
									printf ( "\n" );
								}
							}

							printf ( "\n\n" );
							printf ( "\n---------------------------------------------------------------------\n" );
						//}
#endif

					}
				}

		else
		{
			g_udpSendCount.udpQrecvErrorNum++;
			
			vos_EdrTaskDelay ( 5 );
		}

	}


	close ( SocketFD_I32 );

	return ( OK );

}


INT32 appCanDataSend(char *sendDataPointer,UINT32 sendDataLength )
{
	INT32 retValue = 0;
	
	retValue = vos_EdrMsgQSend(g_RsDataToUdpMsgID,(char *)sendDataPointer,sendDataLength,0,MSG_PRI_NORMAL);
        //printf("retValue is %d \n",retValue);
	
	return retValue;
	
}




STATUS udp_ClientSendTask (void)
{
	STATUS st = ERROR;
	
	UdpDataSendMsgInit();
	st = vos_EdrTaskSpawn ( "t_udpProcessClient", 24, VOS_VX_FP_TASK, 409600, ( FUNCPTR ) udp_ClientProcessSendMain,( INT32 ) NULL );

	if ( st == ERROR )
	{
		printf ( "udp_server process task create failed !\n" );
 
	}

	return st;
}
