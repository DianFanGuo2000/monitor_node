/******************************************************************************
 *File Name	     :	plat_main.c
 *Copyright	     :	Zhuzhou CSR Times Electric Co.,Ltd. All Rights Reserved.
 *Create Date    :	2021/01/18
 *Description    :  
 *
 *REV 1.0.0  DengXiaoOu   2021/01/18  File Create 
 *
 ******************************************************************************/
/*******************************************************************************
 *    Debug switch Section
 ******************************************************************************/

/*******************************************************************************
 *    Include File Section
 ******************************************************************************/

#include "mng_Interface.h"
//#include "app_interface.h"
//#include "mng_MvbInterface.h"
//#include "vos_message.h"
//#include "vos_semaphore.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
//#include <linux/can.h>
//#include <linux/can/raw.h>
#include <sys/select.h> 
#include <termios.h>    /*PPSIX �ն˿��ƶ���*/  
#include <errno.h>      /*����Ŷ���*/

extern void vos_TaskDelay(int );

#define CHANNEL_ONE   (1)
#define CHANNEL_TWO   (2)

#if 0
typedef enum
{
	RET_POINTER_NULL = -1,
	RET_PARAMETER_INVALID = -2;
	
	
	
	
}RET_CODE;

#endif

/*******************************************************************************
 *    Global Variable Define Section
 ******************************************************************************/

/*******************************************************************************
 *    Local Macro Define Section
 ******************************************************************************/
#define MAX_UART_NUM       (1)
/*******************************************************************************
 *    Local Struct  Define Section
 ******************************************************************************/
#pragma pack(1)

typedef struct
{
	UINT32 uartSendOkCnt;
	UINT32 uartSendFailCnt;
	UINT32 uartRecvOkCnt;
	UINT32 uartRecvFailCnt;
}UART_STAT_ST;
#pragma pack()


int plat_uartFdGet(UINT8 uartId);
int cmd_uartDebugGet(UINT8 uartId);

/*******************************************************************************
 *    Local Prototype Declare Section
 ******************************************************************************/

/*******************************************************************************
 *    Static Variable Define Section
 ******************************************************************************/
static int s_fdtty[MAX_UART_NUM] = {0};
static UART_STAT_ST s_uartStatST[MAX_UART_NUM] = {0};
static UINT8 s_uartDebugSwitch[MAX_UART_NUM] = {0};
/*******************************************************************************
 *    Function Define Section
 ******************************************************************************/








/******************************************************************** 
 *Name           :  plat_canInit
 *Function       :  
 *Parameters     :  
 *Return value   :  void
 *Author         :  DengXiaoOu
 *Create Date    :  2020-12-06
 *version        :  1.0.0
***********************************************************************/
/*
c_iflag:����ģʽ��־�������ն����뷽ʽ
IGNBRK     ����BREAK������
BRKINT     ���������IGNBRK��BREAK�������뽫�����ԣ����������BRKINT ��������SIGINT�ж�
IGNPAR     ������żУ�����
PARMRK     ��ʶ��żУ�����
INPCK      ����������żУ��
ISTRIP     ȥ���ַ��ĵ�8������
INLCR      �������NL�����У�ת����CR���س���
IGNCR      ��������Ļس�
ICRNL      ������Ļس�ת���ɻ��У����IGNCRδ���õ�����£�
IUCLC      ������Ĵ�д�ַ�ת����Сд�ַ�����POSIX��
IXON       ��������ʱ��XON/XOFF�����п���
IXANY      �����κ��ַ�������ֹͣ�����
IXOFF      ��������ʱ��XON/XOFF�����п���
IMAXBEL    �������������ʱ��ʼ���壬Linux��ʹ�øò���������Ϊ�ò��������Ѿ�����

c_oflag�����ģʽ��־�������ն������ʽ
OPOST     ��������
OLCUC     �������Сд�ַ�ת���ɴ�д�ַ�����POSIX��
ONLCR     �������NL�����У�ת����CR���س�����NL�����У�
OCRNL     �������CR���س���ת����NL�����У�
ONOCR     ��һ�в�����س���
ONLRET    ������س�
OFILL     ��������ַ����ӳ��ն����
OFDEL     ��ASCII���DEL��Ϊ����ַ������δ���øò���������ַ�����NUL����\0��������POSIX��
NLDLY     ���������ʱ������ȡNL0�����ӳ٣���NL1���ӳ�0.1s��
CRDLY     �س��ӳ٣�ȡֵ��ΧΪ��CR0��CR1��CR2�� CR3
TABDLY    ˮƽ�Ʊ������ӳ٣�ȡֵ��ΧΪ��TAB0��TAB1��TAB2��TAB3
BSDLY     �ո�����ӳ٣�����ȡBS0��BS1
VTDLY     ��ֱ�Ʊ������ӳ٣�����ȡVT0��VT1
FFDLY     ��ҳ�ӳ٣�����ȡFF0��FF1

c_cflag������ģʽ��־��ָ���ն�Ӳ��������Ϣ
CBAUD     �����ʣ�4+1λ������POSIX��
CBAUDEX   ���Ӳ����ʣ�1λ������POSIX��
CSIZE     �ַ����ȣ�ȡֵ��ΧΪCS5��CS6��CS7��CS8
CSTOPB    ��������ֹͣλ
CREAD     ʹ�ý�����
PARENB    ʹ����żУ��
PARODD    ������ʹ����żУ�飬�����ʹ��żУ��
HUPCL     �ر��豸ʱ����
CLOCAL    ���Ե��ƽ������·״̬
CRTSCTS   ʹ��RTS/CTS������

c_lflag������ģʽ��־�������ն˱༭����
ISIG            ������INTR��QUIT��SUSP��DSUSPʱ��������Ӧ���ź�
ICANON          ʹ�ñ�׼����ģʽ
XCASE           ��ICANON��XCASEͬʱ���õ�����£��ն�ֻʹ�ô�д�����ֻ������XCASE���������ַ�����ת��ΪСд�ַ��������ַ�ʹ����ת���ַ�����POSIX����Linux��֧�ָò�����
ECHO            ��ʾ�����ַ�
ECHOE           ���ICANONͬʱ���ã�ERASE��ɾ��������ַ���WERASE��ɾ������ĵ���
ECHOK           ���ICANONͬʱ���ã�KILL��ɾ����ǰ��
ECHONL          ���ICANONͬʱ���ã���ʹECHOû��������Ȼ��ʾ���з�
ECHOPRT         ���ECHO��ICANONͬʱ���ã���ɾ����ӡ�����ַ�����POSIX��
TOSTOP          ���̨�������SIGTTOU�ź�

struct
{
	tcflag_t c_iflag;      
	tcflag_t c_oflag;      
	tcflag_t c_cflag;      
	tcflag_t c_lflag;     
	cc_t     c_cc[NCCS];
} 
*/

int plat_setOptOne(int fd, int speed,int flow_ctrl,int databits,int stopbits,int parity)
{
	struct termios options;
        int ret = -1;
 //       int fd = 0;
	int speed_arr[] = {B115200, B57600, B38400, B19200, B9600, B4800, B2400, B1200, B300};
	int name_arr[] = {115200,  57600, 38400, 19200,  9600,  4800,  2400,  1200,  300};
	int i = 0;
#if 0
    if((uartId < 3) || (uartId > 4))
    {
    	//return RET_PARAMETER_INVAILD;
    	return -2;
    }

    fd =  plat_uartFdGet(uartId);
#endif    
    /*��ȡԭ�д�������*/
    ret = tcgetattr(fd,&options);
    if  (ret != 0) 
    { 
    	printf("tcgetattr failed!!!\n");
        return -1;
    }
    
    //���ô������벨���ʺ����������  
	for ( i= 0;  i < sizeof(speed_arr) / sizeof(int);  i++)  
	{  
		if  (speed == name_arr[i])  
		{               
			cfsetispeed(&options, speed_arr[i]);   
			cfsetospeed(&options, speed_arr[i]);    
		}  
	}
    //�޸Ŀ���ģʽ����֤���򲻻�ռ�ô���  
    options.c_cflag |= CLOCAL; 
    //�޸Ŀ���ģʽ��ʹ���ܹ��Ӵ����ж�ȡ��������  
    options.c_cflag |= CREAD;
    //��������������  
    switch(flow_ctrl)  
    {  
		case 0 ://��ʹ��������  
			options.c_cflag &= ~CRTSCTS;  
              break;     
		case 1 ://ʹ��Ӳ��������  
			options.c_cflag |= CRTSCTS;  
              break;  
		case 2 ://ʹ�����������  
			options.c_cflag |= IXON | IXOFF | IXANY;  
              break;  
    }   
    //��������λ  
	//����������־λ  
	options.c_cflag &= ~CSIZE;  
	switch (databits)  
	{    
		case 5 :  
			options.c_cflag |= CS5;  
			  break;  
		case 6    :  
			options.c_cflag |= CS6;  
			break;  
		case 7    :      
			options.c_cflag |= CS7;  
			break;  
		case 8:      
			options.c_cflag |= CS8;  
			break;    
		default:     
			fprintf(stderr,"Unsupported data size\n");  
			return (FALSE);   
	}    
    // ����ֹͣλ   
    switch (stopbits)  
    {    
		case 1:     
			options.c_cflag &= ~CSTOPB; 
			break;   
		case 2:     
			options.c_cflag |= CSTOPB; 
			break;  
		default:     
			printf("Unsupported stop bits\n");   
			return (FALSE);  
    }  
    //����У��λ  
    switch (parity)  
    {    
		//case 'n':  
		//case 'N': //����żУ��λ�� 
                case 0:  // no parity
			options.c_cflag &= ~PARENB;   
			options.c_iflag &= ~INPCK;      
			break;   
		//case 'o':    
		//case 'O'://����Ϊ��У�� 
                case 1:   // odd    
			options.c_cflag |= (PARODD | PARENB);   
			options.c_iflag |= INPCK;               
			break;   
		//case 'e':   
		//case 'E'://����ΪżУ�� 
                case 2:  // even  
			options.c_cflag |= PARENB;         
			options.c_cflag &= ~PARODD;         
			options.c_iflag |= INPCK;        
			break;  
		//case 's':  
		//case 'S': //����Ϊ�ո� 
                case 3:  //null 
			options.c_cflag &= ~PARENB;  
			options.c_cflag &= ~CSTOPB;  
			break;   
        default:    
        	printf("Unsupported parity\n");      
        	return (FALSE);  
    }  
	//�޸����ģʽ��ԭʼ�������  
	options.c_oflag &= ~OPOST; 
	options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);  
	
	options.c_iflag &= ~(ICRNL | IXON);

    /*���������ַ��͵ȴ�ʱ�䣬���ڽ����ַ��͵ȴ�ʱ��û���ر��Ҫ��ʱ*/
    //newtio.c_cc[VTIME]  = 0;/*�ǹ淶ģʽ��ȡʱ�ĳ�ʱʱ�� */
    //newtio.c_cc[VMIN] = 0;/*�ǹ淶ģʽ��ȡʱ����С�ַ���*/
    options.c_cc[VTIME] = 1; /* ��ȡһ���ַ��ȴ�1*(1/10)s */    
    options.c_cc[VMIN] = 0; /* ��ȡ�ַ������ٸ���Ϊ1 */ 
    
    /*tcflush����ն�δ��ɵ�����/����������ݣ�TCIFLUSH��ʾ������յ������ݣ��Ҳ���ȡ���� */
    tcflush(fd,TCIFLUSH);
    
    //�������� (���޸ĺ��termios�������õ�������)  
    if (tcsetattr(fd,TCSANOW,&options) != 0)    
	{  
		perror("com set error!\n");    
		return (FALSE);   
	}  
    return (TRUE);
}


int plat_setOpt(int speed,int flow_ctrl,int databits,int stopbits,int parity)
{

	struct termios options;
	
    int fd = 0;

    fd =  plat_uartFdGet(3);

    plat_setOptOne(fd,speed,flow_ctrl,databits,stopbits,parity);


}
/******************************************************************** 
 *Name           :  plat_uartInit
 *Function       :  
 *Parameters     :  
 *Return value   :  void
 *Author         :  DengXiaoOu
 *Create Date    :  2020-12-06
 *version        :  1.0.0
***********************************************************************/
int plat_uartInit(char *uartName)
{
	int fd = 0;
		
	fd = open(uartName, O_RDWR|O_NOCTTY|O_NDELAY);
	if(fd < 0)
	{
		printf("open %s failed!!!\n", uartName);
		return FALSE; 
	}
	else
	{
		//printf("open %s success!!!\n", uartName);
	}

	
	//�ָ�����Ϊ����״̬                                 
	if(fcntl(fd, F_SETFL, 0) < 0)  
	{  
		printf("fcntl failed!\n");  
		return FALSE;   
	}       
	else  
	{  
		//printf("fcntl=%d\n",fcntl(fd, F_SETFL,0));  
	}
	//���Դ򿪵��ļ��������Ƿ����ӵ�һ���ն��豸
	/*
	if(0 == isatty(STDIN_FILENO))
	{  
		printf("standard input is not a terminal device\n"); 
		return FALSE;
	}  
	else  
	{  
		printf("isatty success!\n");  
	} 
	*/                 
	
	plat_setOptOne(fd,115200,0,8,1,0);
	return fd;
}





/******************************************************************** 
 *Name           :  APP_uartSend
 *Function       :  
 *Parameters     :  
 *Return value   :  void
 *Author         :  DengXiaoOu
 *Create Date    :  2020-12-06
 *version        :  1.0.0
***********************************************************************/
int plat_uartSend(UINT8 *send_buf, UINT8 data_len)
{
    int len = 0;
    int fd = 0;
	UINT8 uartId = 3;
    
    if(send_buf == NULL)
    {
    	//return RET_POINTER_NULL;
    	return -1;
    }

    fd =  plat_uartFdGet(uartId);
    //printf("send fd is %d ,UartId is %d \n",fd,uartId);
    //vos_TaskDelay(1000);
#if 0
    if(uartId == 3)
    {
    	system("echo 1 > /sys/class/gpio/gpio494/value");
    	system("echo 0 > /sys/class/gpio/gpio397/value");
    	
    }
    else
    {
    	system("echo 1 > /sys/class/gpio/gpio397/value");
    	system("echo 0 > /sys/class/gpio/gpio494/value");
    }
	
    
    vos_TaskDelay(2);
#endif
	
	len = write(fd, send_buf, data_len);
	//vos_TaskDelay(1000);
	
    //system("echo 0 > /sys/class/gpio/gpio494/value");

   // system("echo 0 > /sys/class/gpio/gpio397/value");
    


	if (len == data_len)
	{  
		s_uartStatST[uartId-3].uartSendOkCnt++;
		return 0;
	}       
	else     
	{         
		tcflush(fd,TCOFLUSH);
		s_uartStatST[uartId-3].uartSendFailCnt++;
		return -3;
	}
}



void plat_printf(UINT8 *data,UINT32 len)
{
	int i =0;
	printf("\n");
	for(i=0;i<len;i++)
	{
		printf("[%02x]",data[i]);
		
	}
	printf("\n");
	
}
/******************************************************************** 
 *Name           :  plat_uartRecv
 *Function       :  
 *Parameters     :  
 *Return value   :  void
 *Author         :  DengXiaoOu
 *Create Date    :  2020-12-06
 *version        :  1.0.0
***********************************************************************/
int plat_uartRecv(UINT8 *recv_buf, UINT8 data_len)
{  
	int len = 0;
	int fs_sel = 0;
    fd_set fs_read;
    struct timeval time;
    int fd = 0;
    //int checkFlag = 0;

	UINT8 uartId = 3;
     
    if(recv_buf == NULL)
    {
    	//return RET_POINTER_NULL;
    	return -1;
    }



    fd =  plat_uartFdGet(uartId);
     
    //��˿ڸ��ú���select�ڵ���ǰҪ�������ü����Ķ˿���Ŀ��FD_ZERO����ն˿ڼ���FD_SET�����ö˿ڼ�
    FD_ZERO(&fs_read);  //����׽��ּ���
    FD_SET(fd,&fs_read);  
     
    time.tv_sec = 1;  
    time.tv_usec = 0;  
    
    fs_sel = select(fd+1, &fs_read, NULL, NULL, &time);//ʹ��selectʵ�ִ��ڵĶ�·ͨ��  
    //printf("fs_sel = %d\n",fs_sel);
    if(fs_sel)  
	{  
    	if(FD_ISSET(fd,&fs_read))
		{
    		//checkFlag = plat_checkFrameHeader(uartId, fd);
    		s_uartStatST[uartId-3].uartRecvOkCnt++;
    		len = read(fd, recv_buf, data_len);
    		
    		        //plat_printf(recv_buf, len);
    		
			/*DBG*/
			if(1 == cmd_uartDebugGet(uartId))
			{
				printf("uart :");
				plat_printf(recv_buf, len);
				cmd_uartDebugClose(uartId);
			}
		}
		return len;
	}
    else if(0 == fs_sel)
    {
    	s_uartStatST[uartId-3].uartRecvFailCnt++;
    	//printf("uart%d time out\n", uartId);
    	return -3;
    }
    else  
	{  
    	s_uartStatST[uartId-3].uartRecvFailCnt++;
		printf("sorry,select wrong!");
		return -4;  
	}   
}




/******************************************************************** 
 *Name           :  main_uartTest
 *Function       :  
 *Parameters     :  
 *Return value   :  void
 *Author         :  DengXiaoOu
 *Create Date    :  2021-6-20
 *version        :  1.0.0
***********************************************************************/

#if 0
void main_uartTest(UINT8 sendId, UINT8 recvId)
{
    UINT8 send_buf[10] = {0};
    UINT8 recv_buf[11] = {0};
    UINT8 i = 0;
    int retUart = 0;
    UINT8 errorFlag = 0;
    
	for(i = 0; i < 10; i++)
	{
		send_buf[i] = i;
	}

	//app_uartDataSend(sendId, send_buf, 10);
	retUart = plat_uartSend(sendId, send_buf, 10);
	//printf("send retUart is %d \n",retUart);
	vos_TaskDelay(100);
	//retUart = app_uartDataRecv(recvId, recv_buf, 10);
		
	if(recvId == 1)
	{
		retUart = plat_uartRecv(recvId, recv_buf, 11);
		if(retUart != 11)
		{
			printf("uart%d test failed\n",sendId);
		}
	}
	else
	{

	      retUart = plat_uartRecv(recvId, recv_buf, 10);
		if(retUart != 10)
		{
			printf("uart%d test failed\n",sendId);
		}

	}
	
	
	//printf("recv retUart is %d \n",retUart);
	

	//check
	for(i = 0; i < 10; i++)
	{
		if(recvId == 1)
		{
			if(recv_buf[i+1] != send_buf[i])
			{
				errorFlag = 1;
			}


		}
		else
		{
			if(recv_buf[i] != send_buf[i])
			{
				errorFlag = 1;
			}


		}

	}

	if(errorFlag == 1)
	{
		printf("uart%d test failed\n",sendId);
		plat_printf((UINT8*)&recv_buf, 11);
		errorFlag = 0;
	}
	else
	{
		printf("uart%d test ok\n",sendId);
	}


}

#endif


/*****************************************CMD***************************************/
void cmd_uartInfoShow(void)
{
	int i = 0;
	
	for(i = 0; i < MAX_UART_NUM; i++)
	{
		printf("uart%d info:\n", i+3);
		printf("uart send OK cnt   : %d\n", s_uartStatST[i].uartSendOkCnt);
		printf("uart send fail cnt : %d\n", s_uartStatST[i].uartSendFailCnt);
		printf("uart recv OK cnt   : %d\n", s_uartStatST[i].uartRecvOkCnt);
		printf("uart recv fail cnt : %d\n", s_uartStatST[i].uartRecvFailCnt);
		printf("\n");
	}
}

void cmd_uartDebugOpen(UINT8 uartId)
{
	s_uartDebugSwitch[uartId-3] = 1;
}

void cmd_uartDebugClose(UINT8 uartId)
{
	s_uartDebugSwitch[uartId-3] = 0;
}

int cmd_uartDebugGet(UINT8 uartId)
{
	if(s_uartDebugSwitch[uartId-3] == 1)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void cmd_uart3DebugOpen(void)
{
	cmd_uartDebugOpen(3);
}

void cmd_uart4DebugOpen(void)
{
	cmd_uartDebugOpen(4);
}

void cmd_uart5DebugOpen(void)
{
	cmd_uartDebugOpen(5);
}



#if 0
void gpioInit(void)
{
	system("echo 494 > /sys/class/gpio/export");
	system("echo out > /sys/class/gpio/gpio494/direction");
	
	system("echo 397 > /sys/class/gpio/export");
	system("echo out > /sys/class/gpio/gpio397/direction");
	
	
}

#endif

/******************************************************************** 
 *Name           :  plat_uartTest
 *Function       :  
 *Parameters     :  
 *Return value   :  void
 *Author         :  DengXiaoOu
 *Create Date    :  2021-06-20
 *version        :  1.0.0
***********************************************************************/
void plat_uartTest(void)
{
	//gpioInit();
	
	
	//main_uartTest(1, 2);
       // gpioInit();

     //   main_uartTest(2, 1);
}



void plat_uartAllInit(void)
{
   //gpioInit();

    s_fdtty[0] = plat_uartInit("/dev/ttyS4");
   //printf("s_fdtty[0] is %d \n",s_fdtty[0]);
   // s_fdtty[1] = plat_uartInit("/dev/ttyS2");
  // printf("s_fdtty[1] is %d \n",s_fdtty[1]);
   // //s_fdtty[2] = plat_uartInit("/dev/ttyS5");
}


/******************************************************************** 
 *Name           :  plat_uartFdGet

 *Function       :  
 *Parameters     :  
 *Return value   :  void
 *Author         :  DengXiaoOu
 *Create Date    :  2021-01-22
 *version        :  1.0.0
***********************************************************************/
int plat_uartFdGet(UINT8 uartId)
{
	return s_fdtty[uartId - 3];
}


void uart485Test(void)
{
	plat_uartAllInit();
	
	plat_uartTest();
	
	
	
}

