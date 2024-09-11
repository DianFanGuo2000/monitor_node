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
VOS_EDR_MSG_Q_ID     g_RsDataToUdpCan2MsgID;
VOS_EDR_MSG_Q_ID     g_RsDataToUdpCan3MsgID;
VOS_EDR_MSG_Q_ID     g_RsDataToUdpCan4MsgID;

VOS_EDR_MSG_Q_ID     g_RsDataToUdpCan5MsgID;
VOS_EDR_MSG_Q_ID     g_RsDataToUdpCan6MsgID;
VOS_EDR_MSG_Q_ID     g_RsDataToUdpCan7MsgID;
VOS_EDR_MSG_Q_ID     g_RsDataToUdpCan8MsgID;







//VOS_EDR_MSG_Q_ID     g_udpServerSendMsgID;

#define  UDP_MAX_SEND_LEN   (1432)



struct RS_DATA_SAVE_FRAME_MSG
{


    UINT8  dataArray[200];

};




UDP_SEND_STATISTIC g_udpSendCount = {0};



#pragma pack(1)


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



#pragma pack(0)



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
	
	st = vos_EdrMsgQCreate ( &g_RsDataToUdpMsgID, RSEVTMSGCNTMAX, 1432, VOS_MSG_Q_FIFO, IPC_CREAT | 0660 );

	/*create the msg for send to list*/
	if ( ERROR == st )
	{
		( void ) printf ( "g_RsDataToUdpMsgID create error!\n" );
		return 1U;
	}
	else
	{
	    ;
	}

	return 0U;
}


UINT32 UdpDataSendCan2MsgInit ( void )
{
	STATUS st = ERROR;
	
	st = vos_EdrMsgQCreate ( &g_RsDataToUdpCan2MsgID, RSEVTMSGCNTMAX, 1432, VOS_MSG_Q_FIFO, IPC_CREAT | 0660 );

	/*create the msg for send to list*/
	if ( ERROR == st )
	{
		( void ) printf ( "g_RsDataToUdpCan2MsgID create error!\n" );
		return 1U;
	}
	else
	{
	    ;
	}

	return 0U;
}



UINT32 UdpDataSendCan3MsgInit ( void )
{
	STATUS st = ERROR;
	
	st = vos_EdrMsgQCreate ( &g_RsDataToUdpCan3MsgID, RSEVTMSGCNTMAX, 1432, VOS_MSG_Q_FIFO, IPC_CREAT | 0660 );

	/*create the msg for send to list*/
	if ( ERROR == st )
	{
		( void ) printf ( "g_RsDataToUdpCan3MsgID create error!\n" );
		return 1U;
	}
	else
	{
	    ;
	}

	return 0U;
}




UINT32 UdpDataSendCan4MsgInit ( void )
{
	STATUS st = ERROR;
	
	st = vos_EdrMsgQCreate ( &g_RsDataToUdpCan4MsgID, RSEVTMSGCNTMAX, 1432, VOS_MSG_Q_FIFO, IPC_CREAT | 0660 );

	/*create the msg for send to list*/
	if ( ERROR == st )
	{
		( void ) printf ( "g_RsDataToUdpCan4MsgID create error!\n" );
		return 1U;
	}
	else
	{
	    ;
	}

	return 0U;
}




UINT32 UdpDataSendCan5MsgInit ( void )
{
	STATUS st = ERROR;
	
	st = vos_EdrMsgQCreate ( &g_RsDataToUdpCan5MsgID, RSEVTMSGCNTMAX, 1432, VOS_MSG_Q_FIFO, IPC_CREAT | 0660 );

	/*create the msg for send to list*/
	if ( ERROR == st )
	{
		( void ) printf ( "g_RsDataToUdpCan5MsgID create error!\n" );
		return 1U;
	}
	else
	{
	    ;
	}

	return 0U;
}



UINT32 UdpDataSendCan6MsgInit ( void )
{
	STATUS st = ERROR;
	
	st = vos_EdrMsgQCreate ( &g_RsDataToUdpCan6MsgID, RSEVTMSGCNTMAX, 1432, VOS_MSG_Q_FIFO, IPC_CREAT | 0660 );

	/*create the msg for send to list*/
	if ( ERROR == st )
	{
		( void ) printf ( "g_RsDataToUdpCan6MsgID create error!\n" );
		return 1U;
	}
	else
	{
	    ;
	}

	return 0U;
}





UINT32 UdpDataSendCan7MsgInit ( void )
{
	STATUS st = ERROR;
	
	st = vos_EdrMsgQCreate ( &g_RsDataToUdpCan7MsgID, RSEVTMSGCNTMAX, 1432, VOS_MSG_Q_FIFO, IPC_CREAT | 0660 );

	/*create the msg for send to list*/
	if ( ERROR == st )
	{
		( void ) printf ( "g_RsDataToUdpCan7MsgID create error!\n" );
		return 1U;
	}
	else
	{
	    ;
	}

	return 0U;
}




UINT32 UdpDataSendCan8MsgInit ( void )
{
	STATUS st = ERROR;
	
	st = vos_EdrMsgQCreate ( &g_RsDataToUdpCan8MsgID, RSEVTMSGCNTMAX, 1432, VOS_MSG_Q_FIFO, IPC_CREAT | 0660 );

	/*create the msg for send to list*/
	if ( ERROR == st )
	{
		( void ) printf ( "g_RsDataToUdpCan8MsgID create error!\n" );
		return 1U;
	}
	else
	{
	    ;
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
			
			//g_udpSendCount.udpQrecvRightNum++;
                         //printf("send start ok \r\n");
                         //printf("recvDataLengthValue is %d \r\n",recvDataLengthValue);
                         //printf("sizeof(long) is %d \r\n",sizeof(long));

                          //printf("sizeof(CAN_PROCESS) is %d == 78",sizeof(CAN_PROCESS));

						  /*printf("sended data: ");

                          for(i=0;i<recvDataLengthValue;i++)
{


                           printf("%x ",Data_aC8[i]);


}
printf("\r\n");*/

              
			
					if ( sendto ( SocketFD_I32,(char *)(Data_aC8+sizeof(long)),(recvDataLengthValue-sizeof(long)) , \
					              0, ( struct sockaddr* ) &ClientAddr_ST, SockAddrSize_I32 ) == ERROR )
					{
						
						//g_udpSendCount.udpSendErrorNum++;
                                                printf("udp_ClientProcessSendMain send error \r\n");
						
						close ( SocketFD_I32 );
						return ( ERROR );
					}
					else
					{
                                                printf("udp_ClientProcessSendMain send ok \r\n");
						
						//g_udpSendCount.udpSendRightNum++;

					}
		}

		else
		{
                        printf("recv error !! \r\n");
			//g_udpSendCount.udpQrecvErrorNum++;
			
			vos_EdrTaskDelay ( 5 );
		}

	}


	close ( SocketFD_I32 );

	return ( OK );

}


STATUS udp_ClientProcessCan2SendMain ( void )
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
		if ( ( recvDataLengthValue  = vos_EdrMsgQReceive ( g_RsDataToUdpCan2MsgID, ( void* ) Data_aC8,
		                         UDP_MAX_SEND_LEN, VOS_MSG_WAIT_FOREVER ) ) != ERROR )

		{
			
			//g_udpSendCount.udpQrecvRightNum++;
			
					if ( sendto ( SocketFD_I32,(char *)(Data_aC8+sizeof(long)),(recvDataLengthValue-sizeof(long)) , \
					              0, ( struct sockaddr* ) &ClientAddr_ST, SockAddrSize_I32 ) == ERROR )
					{
						
						//g_udpSendCount.udpSendErrorNum++;
						printf("udp_ClientProcessCan2SendMain send error \r\n");
						close ( SocketFD_I32 );
						return ( ERROR );
					}
					else
					{
						printf("udp_ClientProcessCan2SendMain send ok \r\n");
						//g_udpSendCount.udpSendRightNum++;

					}
				}

		else
		{
			//g_udpSendCount.udpQrecvErrorNum++;
			
			vos_EdrTaskDelay ( 5 );
		}

	}


	close ( SocketFD_I32 );

	return ( OK );

}




STATUS udp_ClientProcessCan3SendMain ( void )
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
		if ( ( recvDataLengthValue  = vos_EdrMsgQReceive ( g_RsDataToUdpCan3MsgID, ( void* ) Data_aC8,
		                         UDP_MAX_SEND_LEN, VOS_MSG_WAIT_FOREVER ) ) != ERROR )

		{
			
			//g_udpSendCount.udpQrecvRightNum++;
			
					if ( sendto ( SocketFD_I32,(char *)(Data_aC8+sizeof(long)),(recvDataLengthValue-sizeof(long)) , \
					              0, ( struct sockaddr* ) &ClientAddr_ST, SockAddrSize_I32 ) == ERROR )
					{
						
						//g_udpSendCount.udpSendErrorNum++;
						printf("udp_ClientProcessCan3SendMain send error \r\n");
						close ( SocketFD_I32 );
						return ( ERROR );
					}
					else
					{
						printf("udp_ClientProcessCan3SendMain send ok \r\n");
						//g_udpSendCount.udpSendRightNum++;

					}
				}

		else
		{
			//g_udpSendCount.udpQrecvErrorNum++;
			
			vos_EdrTaskDelay ( 5 );
		}

	}


	close ( SocketFD_I32 );

	return ( OK );

}





STATUS udp_ClientProcessCan4SendMain ( void )
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
		if ( ( recvDataLengthValue  = vos_EdrMsgQReceive ( g_RsDataToUdpCan4MsgID, ( void* ) Data_aC8,
		                         UDP_MAX_SEND_LEN, VOS_MSG_WAIT_FOREVER ) ) != ERROR )

		{
			
			//g_udpSendCount.udpQrecvRightNum++;
			
					if ( sendto ( SocketFD_I32,(char *)(Data_aC8+sizeof(long)),(recvDataLengthValue-sizeof(long)) , \
					              0, ( struct sockaddr* ) &ClientAddr_ST, SockAddrSize_I32 ) == ERROR )
					{
						
						//g_udpSendCount.udpSendErrorNum++;
						printf("udp_ClientProcessCan4SendMain send error \r\n");
						close ( SocketFD_I32 );
						return ( ERROR );
					}
					else
					{
						printf("udp_ClientProcessCan4SendMain send ok \r\n");
						//g_udpSendCount.udpSendRightNum++;

					}
				}

		else
		{
			//g_udpSendCount.udpQrecvErrorNum++;
			
			vos_EdrTaskDelay ( 5 );
		}

	}


	close ( SocketFD_I32 );

	return ( OK );

}




STATUS udp_ClientProcessCan5SendMain ( void )
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
		if ( ( recvDataLengthValue  = vos_EdrMsgQReceive ( g_RsDataToUdpCan5MsgID, ( void* ) Data_aC8,
		                         UDP_MAX_SEND_LEN, VOS_MSG_WAIT_FOREVER ) ) != ERROR )

		{
			
			//g_udpSendCount.udpQrecvRightNum++;
			
					if ( sendto ( SocketFD_I32,(char *)(Data_aC8+sizeof(long)),(recvDataLengthValue-sizeof(long)) , \
					              0, ( struct sockaddr* ) &ClientAddr_ST, SockAddrSize_I32 ) == ERROR )
					{
						
						//g_udpSendCount.udpSendErrorNum++;
						printf("udp_ClientProcessCan5SendMain send error \r\n");
						close ( SocketFD_I32 );
						return ( ERROR );
					}
					else
					{
						printf("udp_ClientProcessCan5SendMain send ok \r\n");
						//g_udpSendCount.udpSendRightNum++;

					}
				}

		else
		{
			//g_udpSendCount.udpQrecvErrorNum++;
			
			vos_EdrTaskDelay ( 5 );
		}

	}


	close ( SocketFD_I32 );

	return ( OK );

}



STATUS udp_ClientProcessCan6SendMain ( void )
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
		if ( ( recvDataLengthValue  = vos_EdrMsgQReceive ( g_RsDataToUdpCan6MsgID, ( void* ) Data_aC8,
		                         UDP_MAX_SEND_LEN, VOS_MSG_WAIT_FOREVER ) ) != ERROR )

		{
			
			//g_udpSendCount.udpQrecvRightNum++;
			
					if ( sendto ( SocketFD_I32,(char *)(Data_aC8+sizeof(long)),(recvDataLengthValue-sizeof(long)) , \
					              0, ( struct sockaddr* ) &ClientAddr_ST, SockAddrSize_I32 ) == ERROR )
					{
						
						//g_udpSendCount.udpSendErrorNum++;
						printf("udp_ClientProcessCan6SendMain send error \r\n");
						close ( SocketFD_I32 );
						return ( ERROR );
					}
					else
					{
						printf("udp_ClientProcessCan6SendMain send ok \r\n");
						//g_udpSendCount.udpSendRightNum++;

					}
				}

		else
		{
			//g_udpSendCount.udpQrecvErrorNum++;
			
			vos_EdrTaskDelay ( 5 );
		}

	}


	close ( SocketFD_I32 );

	return ( OK );

}


STATUS udp_ClientProcessCan7SendMain ( void )
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
		if ( ( recvDataLengthValue  = vos_EdrMsgQReceive ( g_RsDataToUdpCan7MsgID, ( void* ) Data_aC8,
		                         UDP_MAX_SEND_LEN, VOS_MSG_WAIT_FOREVER ) ) != ERROR )

		{
			
			//g_udpSendCount.udpQrecvRightNum++;
			
					if ( sendto ( SocketFD_I32,(char *)(Data_aC8+sizeof(long)),(recvDataLengthValue-sizeof(long)) , \
					              0, ( struct sockaddr* ) &ClientAddr_ST, SockAddrSize_I32 ) == ERROR )
					{
						
						//g_udpSendCount.udpSendErrorNum++;
						printf("udp_ClientProcessCan7SendMain send error \r\n");
						close ( SocketFD_I32 );
						return ( ERROR );
					}
					else
					{
						printf("udp_ClientProcessCan7SendMain send ok \r\n");
						//g_udpSendCount.udpSendRightNum++;

					}
				}

		else
		{
			//g_udpSendCount.udpQrecvErrorNum++;
			
			vos_EdrTaskDelay ( 5 );
		}

	}


	close ( SocketFD_I32 );

	return ( OK );

}




STATUS udp_ClientProcessCan8SendMain ( void )
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
		if ( ( recvDataLengthValue  = vos_EdrMsgQReceive ( g_RsDataToUdpCan8MsgID, ( void* ) Data_aC8,
		                         UDP_MAX_SEND_LEN, VOS_MSG_WAIT_FOREVER ) ) != ERROR )

		{
			
			//g_udpSendCount.udpQrecvRightNum++;
			
					if ( sendto ( SocketFD_I32,(char *)(Data_aC8+sizeof(long)),(recvDataLengthValue-sizeof(long)) , \
					              0, ( struct sockaddr* ) &ClientAddr_ST, SockAddrSize_I32 ) == ERROR )
					{
						printf("udp_ClientProcessCan8SendMain send error \r\n");
						//g_udpSendCount.udpSendErrorNum++;
						
						close ( SocketFD_I32 );
						return ( ERROR );
					}
					else
					{
						printf("udp_ClientProcessCan8SendMain send ok \r\n");
						//g_udpSendCount.udpSendRightNum++;

					}
				}

		else
		{
			//g_udpSendCount.udpQrecvErrorNum++;
			
			vos_EdrTaskDelay ( 5 );
		}

	}


	close ( SocketFD_I32 );

	return ( OK );

}










INT32 appCanDataSendFunc(UINT32 com,char *sendDataPointer,UINT32 sendDataLength )
{
	INT32 retValue = 0;

	  switch(com)
	  {
  
	  case 1:
		  retValue = vos_EdrMsgQSend(g_RsDataToUdpMsgID,(char *)sendDataPointer,sendDataLength,0,MSG_PRI_NORMAL);
		  break;
	  case 2:
		  retValue = vos_EdrMsgQSend(g_RsDataToUdpCan2MsgID,(char *)sendDataPointer,sendDataLength,0,MSG_PRI_NORMAL);
		  break;
	  case 3:
		  retValue = vos_EdrMsgQSend(g_RsDataToUdpCan3MsgID,(char *)sendDataPointer,sendDataLength,0,MSG_PRI_NORMAL);
		  break;
	  case 4:
		  retValue = vos_EdrMsgQSend(g_RsDataToUdpCan4MsgID,(char *)sendDataPointer,sendDataLength,0,MSG_PRI_NORMAL);
		  break;
	  case 5:
		 retValue = vos_EdrMsgQSend(g_RsDataToUdpCan5MsgID,(char *)sendDataPointer,sendDataLength,0,MSG_PRI_NORMAL);
		  break;
	  case 6:
		  retValue = vos_EdrMsgQSend(g_RsDataToUdpCan6MsgID,(char *)sendDataPointer,sendDataLength,0,MSG_PRI_NORMAL);
		  break;
	  case 7:
		  retValue = vos_EdrMsgQSend(g_RsDataToUdpCan7MsgID,(char *)sendDataPointer,sendDataLength,0,MSG_PRI_NORMAL);
		  break;
	  case 8:
		  retValue = vos_EdrMsgQSend(g_RsDataToUdpCan8MsgID,(char *)sendDataPointer,sendDataLength,0,MSG_PRI_NORMAL);
		  break;
	  	}
	
	return retValue;
	
}




STATUS udp_ClientSendTask (void)
{
	STATUS st = ERROR;
	
printf("udp client semaphore start \r\n");
	UdpDataSendMsgInit();
printf("udp client semaphore end \r\n");
	st = vos_EdrTaskSpawn ( "t_udpProcessClient", 34, VOS_VX_FP_TASK, 40960, ( FUNCPTR ) udp_ClientProcessSendMain,( INT32 ) NULL );

	if ( st == ERROR )
	{
		printf ( "udp_ClientProcessSendMain process task create failed !\n" );
 
	}
else
{

               printf ( "udp_ClientProcessSendMain process task create success !\n" );

}

	return st;
}



STATUS udp_ClientSendCan2Task (void)
{
	STATUS st = ERROR;
	
	UdpDataSendCan2MsgInit();
	st = vos_EdrTaskSpawn ( "t_udpProcessClient", 35, VOS_VX_FP_TASK, 40960, ( FUNCPTR ) udp_ClientProcessCan2SendMain,( INT32 ) NULL );

	if ( st == ERROR )
	{
		printf ( "udp_ClientProcessCan2SendMain process task create failed !\n" );
 
	}

	return st;
}



STATUS udp_ClientSendCan3Task (void)
{
	STATUS st = ERROR;
	
	UdpDataSendCan3MsgInit();
	st = vos_EdrTaskSpawn ( "t_udpProcessClient", 36, VOS_VX_FP_TASK, 409600, ( FUNCPTR ) udp_ClientProcessCan3SendMain,( INT32 ) NULL );

	if ( st == ERROR )
	{
		printf ( "udp_ClientProcessCan3SendMain process task create failed !\n" );
 
	}

	return st;
}


STATUS udp_ClientSendCan4Task (void)
{
	STATUS st = ERROR;
	
	UdpDataSendCan4MsgInit();
	st = vos_EdrTaskSpawn ( "t_udpProcessClient", 37, VOS_VX_FP_TASK, 409600, ( FUNCPTR ) udp_ClientProcessCan4SendMain,( INT32 ) NULL );

	if ( st == ERROR )
	{
		printf ( "udp_ClientProcessCan4SendMain process task create failed !\n" );
 
	}

	return st;
}



STATUS udp_ClientSendCan5Task (void)
{
	STATUS st = ERROR;
	
	UdpDataSendCan5MsgInit();
	st = vos_EdrTaskSpawn ( "t_udpProcessClient", 38, VOS_VX_FP_TASK, 409600, ( FUNCPTR ) udp_ClientProcessCan5SendMain,( INT32 ) NULL );

	if ( st == ERROR )
	{
		printf ( "udp_ClientProcessCan5SendMain process task create failed !\n" );
 
	}

	return st;
}



STATUS udp_ClientSendCan6Task (void)
{
	STATUS st = ERROR;
	
	UdpDataSendCan6MsgInit();
	st = vos_EdrTaskSpawn ( "t_udpProcessClient", 39, VOS_VX_FP_TASK, 409600, ( FUNCPTR ) udp_ClientProcessCan6SendMain,( INT32 ) NULL );

	if ( st == ERROR )
	{
		printf ( "udp_ClientProcessCan6SendMain process task create failed !\n" );
 
	}

	return st;
}



STATUS udp_ClientSendCan7Task (void)
{
	STATUS st = ERROR;
	
	UdpDataSendCan7MsgInit();
	st = vos_EdrTaskSpawn ( "t_udpProcessClient", 40, VOS_VX_FP_TASK, 409600, ( FUNCPTR ) udp_ClientProcessCan7SendMain,( INT32 ) NULL );

	if ( st == ERROR )
	{
		printf ( "udp_ClientProcessCan7SendMain process task create failed !\n" );
 
	}

	return st;
}



STATUS udp_ClientSendCan8Task (void)
{
	STATUS st = ERROR;
	
	UdpDataSendCan8MsgInit();
	st = vos_EdrTaskSpawn ( "t_udpProcessClient", 41, VOS_VX_FP_TASK, 409600, ( FUNCPTR ) udp_ClientProcessCan8SendMain,( INT32 ) NULL );

	if ( st == ERROR )
	{
		printf ( "udp_ClientProcessCan8SendMain process task create failed !\n" );
 
	}

	return st;
}






