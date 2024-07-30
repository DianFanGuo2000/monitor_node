


#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include "udp_Client.h"
#include "udp_Server.h"
#include "string.h"

#include "vos_EdrMessage.h"
#include "mng_Interface.h"
#include "vos_EdrTask.h"




extern UDP_SEND_STATISTIC g_udpSendCount;
extern UDP_RECV_STATISTIC g_udpRecvStatistic;



void getUdpSendStatistic(UINT32 *sendQrecvRightNum,UINT32 *sendQrecvErrorNum,UINT32 *sendUdpRightNum,UINT32 *sendUdpErrorNum)
{	
	*sendQrecvRightNum = g_udpSendCount.udpQrecvRightNum;
	*sendQrecvErrorNum = g_udpSendCount.udpQrecvErrorNum;
	*sendUdpRightNum = g_udpSendCount.udpSendRightNum;
	*sendUdpErrorNum = g_udpSendCount.udpSendErrorNum;	
}



void getUdpRecvStatistic(UINT32 *udpCan1RecvNum,UINT32 *udpCan2RecvNum,UINT32 *udpCan3RecvNum,UINT32 *udpCan4RecvNum,UINT32 *udpCan5RecvNum,UINT32 *udpCanFd1RecvNum,UINT32 *udpCanFd2RecvNum,UINT32 *udpCanFd3RecvNum)
{
	
	*udpCan1RecvNum = g_udpRecvStatistic.udpCan1RecvNum;
	*udpCan2RecvNum = g_udpRecvStatistic.udpCan2RecvNum;
	*udpCan3RecvNum = g_udpRecvStatistic.udpCan3RecvNum;
	*udpCan4RecvNum = g_udpRecvStatistic.udpCan4RecvNum;
	*udpCan5RecvNum = g_udpRecvStatistic.udpCan5RecvNum;
	*udpCanFd1RecvNum = g_udpRecvStatistic.udpCanFd1RecvNum;
	*udpCanFd2RecvNum = g_udpRecvStatistic.udpCanFd2RecvNum;
	*udpCanFd3RecvNum = g_udpRecvStatistic.udpCanFd3RecvNum;
}
