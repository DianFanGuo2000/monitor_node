/******************************************************************************
 *File Name    :    udp_Server.c
 *Copyright    :    Zhuzhou CSR Times Electric Co.,Ltd. All Rights Reserved.
 *Create Date  :    2012/03/15
 *Description  :    UDP Server driver
 *
 *REV1.0.0    Moyun   2012/03/15  File Create
 *
******************************************************************************/
/******************************************************************************
 *    Include File Section
 ******************************************************************************/

#include <sys/socket.h>
#include <netinet/ether.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#include "vos_EdrMessage.h"
#include "vos_EdrTask.h"
#include "vos_EdrSemaphore.h"

#include "string.h"
#include "udp_Server.h"


extern INT32  vos_EdrSemPost(VOS_EDR_SEM_ID* Sem_pId);
extern INT32 vos_EdrSemWait(VOS_EDR_SEM_ID *Sem_pId,INT32 Timeout_I32);


//extern VOS_EDR_MSG_Q_ID     g_udpServerSendMsgID;


#define   RECV_RESPONSE_TYPE    ("CFGR")
#define   RECV_PROCESS_TYPE     ("CIN")

#define   RECV_FRAME_TYPE_LENGTH   (4)







INT32 udp_RecvConfigResponseFun(char *pointer);
INT32  udp_RecvProcessDataFun(char *pointer);


typedef  INT32 (*udpServerCallBackFunc)(char *data,UINT32 dataLength);

UDP_RECV_STATISTIC g_udpRecvStatistic = {0};




typedef struct _UDP_PORT_INFO
{
	
	UINT32 port;
	UINT32 *semaphorePointer;
	
}UDP_PORT_INFO;



typedef struct _UDP_RECV_INFO
{
	char recvFrameType[4];
	INT32 (*udp_RecvFun)(char *pointer);
}UDP_RECV_INFO;





UDP_RECV_INFO  g_UdpRecvInfoFun_aST[] =
{
{ RECV_RESPONSE_TYPE, udp_RecvConfigResponseFun },
{ RECV_PROCESS_TYPE, udp_RecvProcessDataFun } 
};





INT32 udp_RecvConfigResponseFun(char *pointer)
{
	
	
	
}





INT32  udp_RecvProcessDataFun(char *pointer)
{
	
	
}


/******************************************************************************
 *    Debug switch Section
 ******************************************************************************/

/******************************************************************************
 *    Global Variable Define Section
 ******************************************************************************/



/******************************************************************************
 *    Local Macro Define Section
 ******************************************************************************/


/******************************************************************************
 *    Local Struct  Define Section
 ******************************************************************************/


static int g_ServerDebugMode_I32 = 1;


udpServerCallBackFunc g_callBack = NULL;

udpServerCallBackFunc g_callBackCanFd1 = NULL;
udpServerCallBackFunc g_callBackCanFd2 = NULL;
udpServerCallBackFunc g_callBackCanFd3 = NULL;





void setUdpServerCallBackFunc(udpServerCallBackFunc callBackFuc)
{
	
	g_callBack = callBackFuc;
}



void setUdpServerCanFd1CallBackFunc(udpServerCallBackFunc callBackFuc)
{
	
	g_callBackCanFd1 = callBackFuc;
}


void setUdpServerCanFd2CallBackFunc(udpServerCallBackFunc callBackFuc)
{
	
	g_callBackCanFd2 = callBackFuc;
}


void setUdpServerCanFd3CallBackFunc(udpServerCallBackFunc callBackFuc)
{
	
	g_callBackCanFd3 = callBackFuc;
}




/******************************************************************************
 *    Local Prototype Declare Section
 ******************************************************************************/

/******************************************************************************
 *    Static Variable Define Section
 ******************************************************************************/


/******************************************************************************
 *    Function Define Section
 ******************************************************************************/


#if 0

void UDP_RecvInfoProcessFuc(char *pointer)
{
	UINT8 i = 0;
	
	for(i=0;i<(sizeof(g_UdpRecvInfoFun_aST)/sizeof(g_UdpRecvInfoFun_aST[0]));i++)
	{			
		if(strncmp(g_UdpRecvInfoFun_aST[i].recvFrameType,pointer,RECV_FRAME_TYPE_LENGTH) == 0)
		{
			
			return g_UdpRecvInfoFun_aST[i].udp_RecvFun(pointer);
			
		}
		
	}
	
}

#endif


/******************************************************************************
 * Name: STATUS udp_ServerMain(void)
 * Function: the udp server create function.
 * Information: the udp server create function
 * Input:  non
 * return : OK,if the udp client create successed,otherwise return ERROR.
 * Create by moyun  2018-01-25
 ******************************************************************************/
STATUS udp_ServerProcessRecvMain ()
{
	struct sockaddr_in ServerAddr_ST;/*server's socker address*/
	struct sockaddr_in ClientAddr_ST;/*client's socket address*/
	INT32 SockAddrSize_I32 = 0;
	INT32 SFd_I32 = 0;
	INT32 i = 0;
	INT8 InetAddr_pI8[INET_ADDR_LEN] = {0};
	INT8 McastIp_pI8[INET_ADDR_LEN] = {0};
	INT8 HostIpAddr_pI8[INET_ADDR_LEN] = {0};
	INT32 ReceiveDataLen_I32 = 0;
	UINT32 ReceiveDataLen_U32 = 0;
	UINT8 ReceiveBuffer_pU8[UDP_MAX_RECEIVE_LEN] = {0};
	UINT32 RecvBufLen_U32 = 0;
	
	INT32 Port_I32 = UDP_RECV_PORT;

	INT32 TheMutiTTLVal_I32 = UDP_MULTI_TTL_VAL;
	INT32 TheSingleTTLVal_I32 = UDP_MULTI_TTL_VAL;
	struct ip_mreq ipMreq;
	char UdpData_aC8[1080] = {0};
	int DataLen_I32 = 0;

	SockAddrSize_I32 = sizeof ( struct sockaddr_in );
	bzero ( ( char* ) &ServerAddr_ST, SockAddrSize_I32 );
	bzero ( ( char* ) &ClientAddr_ST, SockAddrSize_I32 );
	ServerAddr_ST.sin_family = AF_INET;
	
	
	ServerAddr_ST.sin_port = htons ( Port_I32 );
	
	ServerAddr_ST.sin_addr.s_addr = htonl ( INADDR_ANY );

	//ServerAddr_ST.sin_addr.s_addr = g_udpLinkRecvPortIp_U32;
	
	SFd_I32 = socket ( AF_INET, SOCK_DGRAM, 0 );

	if ( SFd_I32 == ERROR )
	{
		perror ( "socket" );
		return ( ERROR );
	}

	/*add ttl 2012-12-27*/
	if ( setsockopt ( SFd_I32, IPPROTO_IP, IP_MULTICAST_TTL, ( char* ) &TheMutiTTLVal_I32,
	                  sizeof ( TheMutiTTLVal_I32 ) ) < 0 )
	{
		printf ( "setsockopt Set TTL error:\n" );

		return ( ERROR );

	}

	/*add single ttl 2012-03-29*/
	if ( setsockopt ( SFd_I32, IPPROTO_IP, IP_TTL, ( char* ) &TheSingleTTLVal_I32,
	                  sizeof ( TheSingleTTLVal_I32 ) ) < 0 )
	{
		printf ( "setsockopt Set single TTL error:\n" );
		return ( ERROR );
	}

	if ( bind ( SFd_I32, ( struct sockaddr* ) ( ( &ServerAddr_ST ) ), SockAddrSize_I32 ) == ERROR )
	{
		perror ( "bind" );
		close ( SFd_I32 );
		return ( ERROR );
	}

	while ( 1 )
	{
		memset ( ReceiveBuffer_pU8, 0, UDP_MAX_RECEIVE_LEN );

		if ( ( ReceiveDataLen_I32 = recvfrom ( SFd_I32, ( char* ) ReceiveBuffer_pU8,
		                                       UDP_MAX_RECEIVE_LEN, 0,
		                                       ( struct sockaddr* ) &ServerAddr_ST, &SockAddrSize_I32 ) ) == ERROR )
		{
			
			perror ( "recvfrom" );
			close ( SFd_I32 );
			return ( ERROR );
		}

		g_udpRecvStatistic.udpCanRecvNum++;
		
		/*this is the ip address.*/
		/*get the ip address from the server . add by moyun 2018-01-23.
		 first it should be get the ip address,and then create the client,used to send data. the client and the
		 server  comunication by the message. add by moyun 2018-01-23.
		 there are some problem .how to deal with the ip address, the ip address is not the same ip address.
		   every time, so if the ip address is not same ,it should be recreate the client.
		   how to deal with this problem? */
		//strncpy ( InetAddr_pI8, inet_ntoa ( ClientAddr_ST.sin_addr ), INET_ADDR_LEN );

		ReceiveDataLen_U32 = ( UINT32 ) ReceiveDataLen_I32;
		
		if(g_callBack)
		{
			g_callBack((char *)ReceiveBuffer_pU8,ReceiveDataLen_I32);	
		}
			
	}

	close ( SFd_I32 );

	return ( OK );
	/*the udp client,send the data only,the udp server receive the data only,after the udp server receive the data,
	there are a function deal with the data,send the data to the rs422,and then receive the udp data to the udp port.
	the udp server function should be parse the data,and send the data to the udp.
	when the connect come,i receive the data only,and tell master contoller the message .
	*/

}


/******************************************************************************
 * Name: STATUS udp_ServerMain(void)
 * Function: the udp server create function.
 * Information: the udp server create function
 * Input:  non
 * return : OK,if the udp client create successed,otherwise return ERROR.
 * Create by moyun  2018-01-25
 ******************************************************************************/
STATUS udp_ServerProcessCanFd1RecvMain ()
{
	struct sockaddr_in ServerAddr_ST;/*server's socker address*/
	struct sockaddr_in ClientAddr_ST;/*client's socket address*/
	INT32 SockAddrSize_I32 = 0;
	INT32 SFd_I32 = 0;
	INT32 i = 0;
	INT8 InetAddr_pI8[INET_ADDR_LEN] = {0};
	INT8 McastIp_pI8[INET_ADDR_LEN] = {0};
	INT8 HostIpAddr_pI8[INET_ADDR_LEN] = {0};
	INT32 ReceiveDataLen_I32 = 0;
	UINT32 ReceiveDataLen_U32 = 0;
	UINT8 ReceiveBuffer_pU8[UDP_MAX_RECEIVE_LEN] = {0};
	UINT32 RecvBufLen_U32 = 0;
	
	INT32 Port_I32 = UDP_RECV_CANFD1_PORT;

	INT32 TheMutiTTLVal_I32 = UDP_MULTI_TTL_VAL;
	INT32 TheSingleTTLVal_I32 = UDP_MULTI_TTL_VAL;
	struct ip_mreq ipMreq;
	char UdpData_aC8[1080] = {0};
	int DataLen_I32 = 0;

	SockAddrSize_I32 = sizeof ( struct sockaddr_in );
	bzero ( ( char* ) &ServerAddr_ST, SockAddrSize_I32 );
	bzero ( ( char* ) &ClientAddr_ST, SockAddrSize_I32 );
	ServerAddr_ST.sin_family = AF_INET;
	
	
	ServerAddr_ST.sin_port = htons ( Port_I32 );
	
	ServerAddr_ST.sin_addr.s_addr = htonl ( INADDR_ANY );

	//ServerAddr_ST.sin_addr.s_addr = g_udpLinkRecvPortIp_U32;
	
	SFd_I32 = socket ( AF_INET, SOCK_DGRAM, 0 );

	if ( SFd_I32 == ERROR )
	{
		perror ( "socket" );
		return ( ERROR );
	}

	/*add ttl 2012-12-27*/
	if ( setsockopt ( SFd_I32, IPPROTO_IP, IP_MULTICAST_TTL, ( char* ) &TheMutiTTLVal_I32,
	                  sizeof ( TheMutiTTLVal_I32 ) ) < 0 )
	{
		printf ( "setsockopt Set TTL error:\n" );

		return ( ERROR );

	}

	/*add single ttl 2012-03-29*/
	if ( setsockopt ( SFd_I32, IPPROTO_IP, IP_TTL, ( char* ) &TheSingleTTLVal_I32,
	                  sizeof ( TheSingleTTLVal_I32 ) ) < 0 )
	{
		printf ( "setsockopt Set single TTL error:\n" );
		return ( ERROR );
	}

	if ( bind ( SFd_I32, ( struct sockaddr* ) ( ( &ServerAddr_ST ) ), SockAddrSize_I32 ) == ERROR )
	{
		perror ( "bind" );
		close ( SFd_I32 );
		return ( ERROR );
	}

	while ( 1 )
	{
		memset ( ReceiveBuffer_pU8, 0, UDP_MAX_RECEIVE_LEN );

		if ( ( ReceiveDataLen_I32 = recvfrom ( SFd_I32, ( char* ) ReceiveBuffer_pU8,
		                                       UDP_MAX_RECEIVE_LEN, 0,
		                                       ( struct sockaddr* ) &ServerAddr_ST, &SockAddrSize_I32 ) ) == ERROR )
		{
			perror ( "recvfrom" );
			close ( SFd_I32 );
			return ( ERROR );
		}

		g_udpRecvStatistic.udpCanFd1RecvNum++;
		
		ReceiveDataLen_U32 = ( UINT32 ) ReceiveDataLen_I32;
		
		if(g_callBackCanFd1)
		{
			g_callBackCanFd1((char *)ReceiveBuffer_pU8,ReceiveDataLen_I32);	
		}

			
	}

	close ( SFd_I32 );

	return ( OK );

}


/******************************************************************************
 * Name: STATUS udp_ServerMain(void)
 * Function: the udp server create function.
 * Information: the udp server create function
 * Input:  non
 * return : OK,if the udp client create successed,otherwise return ERROR.
 * Create by moyun  2018-01-25
 ******************************************************************************/
STATUS udp_ServerProcessCanFd2RecvMain ()
{
	struct sockaddr_in ServerAddr_ST;/*server's socker address*/
	struct sockaddr_in ClientAddr_ST;/*client's socket address*/
	INT32 SockAddrSize_I32 = 0;
	INT32 SFd_I32 = 0;
	INT32 i = 0;
	INT8 InetAddr_pI8[INET_ADDR_LEN] = {0};
	INT8 McastIp_pI8[INET_ADDR_LEN] = {0};
	INT8 HostIpAddr_pI8[INET_ADDR_LEN] = {0};
	INT32 ReceiveDataLen_I32 = 0;
	UINT32 ReceiveDataLen_U32 = 0;
	UINT8 ReceiveBuffer_pU8[UDP_MAX_RECEIVE_LEN] = {0};
	UINT32 RecvBufLen_U32 = 0;
	
	INT32 Port_I32 = UDP_RECV_CANFD2_PORT;

	INT32 TheMutiTTLVal_I32 = UDP_MULTI_TTL_VAL;
	INT32 TheSingleTTLVal_I32 = UDP_MULTI_TTL_VAL;
	struct ip_mreq ipMreq;
	char UdpData_aC8[1080] = {0};
	int DataLen_I32 = 0;

	SockAddrSize_I32 = sizeof ( struct sockaddr_in );
	bzero ( ( char* ) &ServerAddr_ST, SockAddrSize_I32 );
	bzero ( ( char* ) &ClientAddr_ST, SockAddrSize_I32 );
	ServerAddr_ST.sin_family = AF_INET;
	
	
	ServerAddr_ST.sin_port = htons ( Port_I32 );
	
	ServerAddr_ST.sin_addr.s_addr = htonl ( INADDR_ANY );

	//ServerAddr_ST.sin_addr.s_addr = g_udpLinkRecvPortIp_U32;
	
	SFd_I32 = socket ( AF_INET, SOCK_DGRAM, 0 );

	if ( SFd_I32 == ERROR )
	{
		perror ( "socket" );
		return ( ERROR );
	}

	/*add ttl 2012-12-27*/
	if ( setsockopt ( SFd_I32, IPPROTO_IP, IP_MULTICAST_TTL, ( char* ) &TheMutiTTLVal_I32,
	                  sizeof ( TheMutiTTLVal_I32 ) ) < 0 )
	{
		printf ( "setsockopt Set TTL error:\n" );

		return ( ERROR );

	}

	/*add single ttl 2012-03-29*/
	if ( setsockopt ( SFd_I32, IPPROTO_IP, IP_TTL, ( char* ) &TheSingleTTLVal_I32,
	                  sizeof ( TheSingleTTLVal_I32 ) ) < 0 )
	{
		printf ( "setsockopt Set single TTL error:\n" );
		return ( ERROR );
	}

	if ( bind ( SFd_I32, ( struct sockaddr* ) ( ( &ServerAddr_ST ) ), SockAddrSize_I32 ) == ERROR )
	{
		perror ( "bind" );
		close ( SFd_I32 );
		return ( ERROR );
	}

	while ( 1 )
	{
		memset ( ReceiveBuffer_pU8, 0, UDP_MAX_RECEIVE_LEN );

		if ( ( ReceiveDataLen_I32 = recvfrom ( SFd_I32, ( char* ) ReceiveBuffer_pU8,
		                                       UDP_MAX_RECEIVE_LEN, 0,
		                                       ( struct sockaddr* ) &ServerAddr_ST, &SockAddrSize_I32 ) ) == ERROR )
		{
			perror ( "recvfrom" );
			close ( SFd_I32 );
			return ( ERROR );
		}

		g_udpRecvStatistic.udpCanFd2RecvNum++;
		
		ReceiveDataLen_U32 = ( UINT32 ) ReceiveDataLen_I32;
		
		if(g_callBackCanFd2)
		{
			g_callBackCanFd2((char *)ReceiveBuffer_pU8,ReceiveDataLen_I32);	
		}
			
	}

	close ( SFd_I32 );

	return ( OK );

}


/******************************************************************************
 * Name: STATUS udp_ServerMain(void)
 * Function: the udp server create function.
 * Information: the udp server create function
 * Input:  non
 * return : OK,if the udp client create successed,otherwise return ERROR.
 * Create by moyun  2018-01-25
 ******************************************************************************/
STATUS udp_ServerProcessCanFd3RecvMain ()
{
	struct sockaddr_in ServerAddr_ST;/*server's socker address*/
	struct sockaddr_in ClientAddr_ST;/*client's socket address*/
	INT32 SockAddrSize_I32 = 0;
	INT32 SFd_I32 = 0;
	INT32 i = 0;
	INT8 InetAddr_pI8[INET_ADDR_LEN] = {0};
	INT8 McastIp_pI8[INET_ADDR_LEN] = {0};
	INT8 HostIpAddr_pI8[INET_ADDR_LEN] = {0};
	INT32 ReceiveDataLen_I32 = 0;
	UINT32 ReceiveDataLen_U32 = 0;
	UINT8 ReceiveBuffer_pU8[UDP_MAX_RECEIVE_LEN] = {0};
	UINT32 RecvBufLen_U32 = 0;
	
	INT32 Port_I32 = UDP_RECV_CANFD3_PORT;

	INT32 TheMutiTTLVal_I32 = UDP_MULTI_TTL_VAL;
	INT32 TheSingleTTLVal_I32 = UDP_MULTI_TTL_VAL;
	struct ip_mreq ipMreq;
	char UdpData_aC8[1080] = {0};
	int DataLen_I32 = 0;

	SockAddrSize_I32 = sizeof ( struct sockaddr_in );
	bzero ( ( char* ) &ServerAddr_ST, SockAddrSize_I32 );
	bzero ( ( char* ) &ClientAddr_ST, SockAddrSize_I32 );
	ServerAddr_ST.sin_family = AF_INET;
	
	
	ServerAddr_ST.sin_port = htons ( Port_I32 );
	
	ServerAddr_ST.sin_addr.s_addr = htonl ( INADDR_ANY );

	//ServerAddr_ST.sin_addr.s_addr = g_udpLinkRecvPortIp_U32;
	
	SFd_I32 = socket ( AF_INET, SOCK_DGRAM, 0 );

	if ( SFd_I32 == ERROR )
	{
		perror ( "socket" );
		return ( ERROR );
	}

	/*add ttl 2012-12-27*/
	if ( setsockopt ( SFd_I32, IPPROTO_IP, IP_MULTICAST_TTL, ( char* ) &TheMutiTTLVal_I32,
	                  sizeof ( TheMutiTTLVal_I32 ) ) < 0 )
	{
		printf ( "setsockopt Set TTL error:\n" );

		return ( ERROR );

	}

	/*add single ttl 2012-03-29*/
	if ( setsockopt ( SFd_I32, IPPROTO_IP, IP_TTL, ( char* ) &TheSingleTTLVal_I32,
	                  sizeof ( TheSingleTTLVal_I32 ) ) < 0 )
	{
		printf ( "setsockopt Set single TTL error:\n" );
		return ( ERROR );
	}

	if ( bind ( SFd_I32, ( struct sockaddr* ) ( ( &ServerAddr_ST ) ), SockAddrSize_I32 ) == ERROR )
	{
		perror ( "bind" );
		close ( SFd_I32 );
		return ( ERROR );
	}

	while ( 1 )
	{
		memset ( ReceiveBuffer_pU8, 0, UDP_MAX_RECEIVE_LEN );

		if ( ( ReceiveDataLen_I32 = recvfrom ( SFd_I32, ( char* ) ReceiveBuffer_pU8,
		                                       UDP_MAX_RECEIVE_LEN, 0,
		                                       ( struct sockaddr* ) &ServerAddr_ST, &SockAddrSize_I32 ) ) == ERROR )
		{
			perror ( "recvfrom" );
			close ( SFd_I32 );
			return ( ERROR );
		}
		
		g_udpRecvStatistic.udpCanFd3RecvNum++;

		ReceiveDataLen_U32 = ( UINT32 ) ReceiveDataLen_I32;
		
		if(g_callBackCanFd3)
		{
			g_callBackCanFd3((char *)ReceiveBuffer_pU8,ReceiveDataLen_I32);	
		}
			
	}

	close ( SFd_I32 );

	return ( OK );

}

#if 0
void udpSendMsgInit(void)
{
	
	STATUS st = ERROR;
		
	// printf("udp send msg create \n");
	st = vos_EdrMsgQCreate ( &g_udpServerSendMsgID, RSEVTMSGCNTMAX, 1432, VOS_MSG_Q_FIFO, IPC_CREAT | 0660 );

	/*create the msg for send to list*/
	if ( ERROR == st )
	{
		( void ) printf ( "g_udpSendMsgID create error!\n" );
		return 1U;
	}
	else
	{
		printf("udp server send msg create ok \n");
	}

	return 0U;
	
	
	
}


#endif
/******************************************************************************
 * Name: STATUS udp_ServerTask(void)
 * Function: the udp server task.
 * Information: start the udp server task.
 * Input: non.
 * return :if the task create ok,return true,otherwise return ERROR;
 * Create by moyun  2018-01-25
 ******************************************************************************/


STATUS udp_ServerRecvTask (void)
{
	STATUS st = ERROR;


	//udpSendMsgInit();
	st = vos_EdrTaskSpawn ( "t_udp", 23, VOS_VX_FP_TASK, 40960, ( FUNCPTR ) udp_ServerProcessRecvMain,( INT32 ) NULL );

	if ( st == ERROR )
	{
		printf ( "udp_server process task create failed !\n" );
 
	}
	
	
	st = vos_EdrTaskSpawn ( "t_udpCanFd1", 24, VOS_VX_FP_TASK, 40960, ( FUNCPTR ) udp_ServerProcessCanFd1RecvMain,( INT32 ) NULL );

	if ( st == ERROR )
	{
		printf ( "t_udpCanFd1ProcessServer process task create failed !\n" );
 
	}
	
	st = vos_EdrTaskSpawn ( "t_udpCanFd2", 25, VOS_VX_FP_TASK, 40960, ( FUNCPTR ) udp_ServerProcessCanFd2RecvMain,( INT32 ) NULL );

	if ( st == ERROR )
	{
		printf ( "t_udpCanFd2ProcessServer process task create failed !\n" );
 
	}
	
	st = vos_EdrTaskSpawn ( "t_udpCanFd3", 26, VOS_VX_FP_TASK, 40960, ( FUNCPTR ) udp_ServerProcessCanFd3RecvMain,( INT32 ) NULL );

	if ( st == ERROR )
	{
		printf ( "t_udpCanFd3ProcessServer process task create failed !\n" );
 
	}

	return st;
}
