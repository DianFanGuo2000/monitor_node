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
#include <termios.h>    /*PPSIX 终端控制定义*/  
#include <errno.h>      /*错误号定义*/

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
c_iflag:输入模式标志，控制终端输入方式
IGNBRK     忽略BREAK键输入
BRKINT     如果设置了IGNBRK，BREAK键的输入将被忽略，如果设置了BRKINT ，将产生SIGINT中断
IGNPAR     忽略奇偶校验错误
PARMRK     标识奇偶校验错误
INPCK      允许输入奇偶校验
ISTRIP     去除字符的第8个比特
INLCR      将输入的NL（换行）转换成CR（回车）
IGNCR      忽略输入的回车
ICRNL      将输入的回车转化成换行（如果IGNCR未设置的情况下）
IUCLC      将输入的大写字符转换成小写字符（非POSIX）
IXON       允许输入时对XON/XOFF流进行控制
IXANY      输入任何字符将重启停止的输出
IXOFF      允许输入时对XON/XOFF流进行控制
IMAXBEL    当输入队列满的时候开始响铃，Linux在使用该参数而是认为该参数总是已经设置

c_oflag：输出模式标志，控制终端输出方式
OPOST     处理后输出
OLCUC     将输入的小写字符转换成大写字符（非POSIX）
ONLCR     将输入的NL（换行）转换成CR（回车）及NL（换行）
OCRNL     将输入的CR（回车）转换成NL（换行）
ONOCR     第一行不输出回车符
ONLRET    不输出回车
OFILL     发送填充字符以延迟终端输出
OFDEL     以ASCII码的DEL作为填充字符，如果未设置该参数，填充字符将是NUL（‘\0’）（非POSIX）
NLDLY     换行输出延时，可以取NL0（不延迟）或NL1（延迟0.1s）
CRDLY     回车延迟，取值范围为：CR0、CR1、CR2和 CR3
TABDLY    水平制表符输出延迟，取值范围为：TAB0、TAB1、TAB2和TAB3
BSDLY     空格输出延迟，可以取BS0或BS1
VTDLY     垂直制表符输出延迟，可以取VT0或VT1
FFDLY     换页延迟，可以取FF0或FF1

c_cflag：控制模式标志，指定终端硬件控制信息
CBAUD     波特率（4+1位）（非POSIX）
CBAUDEX   附加波特率（1位）（非POSIX）
CSIZE     字符长度，取值范围为CS5、CS6、CS7或CS8
CSTOPB    设置两个停止位
CREAD     使用接收器
PARENB    使用奇偶校验
PARODD    对输入使用奇偶校验，对输出使用偶校验
HUPCL     关闭设备时挂起
CLOCAL    忽略调制解调器线路状态
CRTSCTS   使用RTS/CTS流控制

c_lflag：本地模式标志，控制终端编辑功能
ISIG            当输入INTR、QUIT、SUSP或DSUSP时，产生相应的信号
ICANON          使用标准输入模式
XCASE           在ICANON和XCASE同时设置的情况下，终端只使用大写。如果只设置了XCASE，则输入字符将被转换为小写字符，除非字符使用了转义字符（非POSIX，且Linux不支持该参数）
ECHO            显示输入字符
ECHOE           如果ICANON同时设置，ERASE将删除输入的字符，WERASE将删除输入的单词
ECHOK           如果ICANON同时设置，KILL将删除当前行
ECHONL          如果ICANON同时设置，即使ECHO没有设置依然显示换行符
ECHOPRT         如果ECHO和ICANON同时设置，将删除打印出的字符（非POSIX）
TOSTOP          向后台输出发送SIGTTOU信号

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
    /*获取原有串口配置*/
    ret = tcgetattr(fd,&options);
    if  (ret != 0) 
    { 
    	printf("tcgetattr failed!!!\n");
        return -1;
    }
    
    //设置串口输入波特率和输出波特率  
	for ( i= 0;  i < sizeof(speed_arr) / sizeof(int);  i++)  
	{  
		if  (speed == name_arr[i])  
		{               
			cfsetispeed(&options, speed_arr[i]);   
			cfsetospeed(&options, speed_arr[i]);    
		}  
	}
    //修改控制模式，保证程序不会占用串口  
    options.c_cflag |= CLOCAL; 
    //修改控制模式，使得能够从串口中读取输入数据  
    options.c_cflag |= CREAD;
    //设置数据流控制  
    switch(flow_ctrl)  
    {  
		case 0 ://不使用流控制  
			options.c_cflag &= ~CRTSCTS;  
              break;     
		case 1 ://使用硬件流控制  
			options.c_cflag |= CRTSCTS;  
              break;  
		case 2 ://使用软件流控制  
			options.c_cflag |= IXON | IXOFF | IXANY;  
              break;  
    }   
    //设置数据位  
	//屏蔽其他标志位  
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
    // 设置停止位   
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
    //设置校验位  
    switch (parity)  
    {    
		//case 'n':  
		//case 'N': //无奇偶校验位。 
                case 0:  // no parity
			options.c_cflag &= ~PARENB;   
			options.c_iflag &= ~INPCK;      
			break;   
		//case 'o':    
		//case 'O'://设置为奇校验 
                case 1:   // odd    
			options.c_cflag |= (PARODD | PARENB);   
			options.c_iflag |= INPCK;               
			break;   
		//case 'e':   
		//case 'E'://设置为偶校验 
                case 2:  // even  
			options.c_cflag |= PARENB;         
			options.c_cflag &= ~PARODD;         
			options.c_iflag |= INPCK;        
			break;  
		//case 's':  
		//case 'S': //设置为空格 
                case 3:  //null 
			options.c_cflag &= ~PARENB;  
			options.c_cflag &= ~CSTOPB;  
			break;   
        default:    
        	printf("Unsupported parity\n");      
        	return (FALSE);  
    }  
	//修改输出模式，原始数据输出  
	options.c_oflag &= ~OPOST; 
	options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);  
	
	options.c_iflag &= ~(ICRNL | IXON);

    /*设置最少字符和等待时间，对于接收字符和等待时间没有特别的要求时*/
    //newtio.c_cc[VTIME]  = 0;/*非规范模式读取时的超时时间 */
    //newtio.c_cc[VMIN] = 0;/*非规范模式读取时的最小字符数*/
    options.c_cc[VTIME] = 1; /* 读取一个字符等待1*(1/10)s */    
    options.c_cc[VMIN] = 0; /* 读取字符的最少个数为1 */ 
    
    /*tcflush清空终端未完成的输入/输出请求及数据；TCIFLUSH表示清空正收到的数据，且不读取出来 */
    tcflush(fd,TCIFLUSH);
    
    //激活配置 (将修改后的termios数据设置到串口中)  
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

	
	//恢复串口为阻塞状态                                 
	if(fcntl(fd, F_SETFL, 0) < 0)  
	{  
		printf("fcntl failed!\n");  
		return FALSE;   
	}       
	else  
	{  
		//printf("fcntl=%d\n",fcntl(fd, F_SETFL,0));  
	}
	//测试打开的文件描述符是否链接到一个终端设备
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
     
    //多端口复用函数select在调用前要首先设置监听的端口数目，FD_ZERO是清空端口集，FD_SET是设置端口集
    FD_ZERO(&fs_read);  //清空套接字集合
    FD_SET(fd,&fs_read);  
     
    time.tv_sec = 1;  
    time.tv_usec = 0;  
    
    fs_sel = select(fd+1, &fs_read, NULL, NULL, &time);//使用select实现串口的多路通信  
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

