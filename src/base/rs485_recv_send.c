

#include "rs485_recv_send.h"


int speed_arr[] = {B921600, B460800, B230400, B115200, B38400, B19200, B9600, B4800, B2400, B1200, B300};
int name_arr[] = {921600, 460800, 230400, 115200, 38400, 19200, 9600, 4800, 2400, 1200, 300};

// Function to export a GPIO pin to the user space  
void exportGPIO(int rs485_gpio_number)  
{  
    char gpio_path[64]; // Assuming GPIO number won't exceed 5 digits + extra for path  
    char export_cmd[128]; // Command buffer to hold the echo command  
  
    // Construct the path to check if the GPIO is already exported  
    snprintf(gpio_path, sizeof(gpio_path), "/sys/class/gpio/gpio%d", rs485_gpio_number);  
  
    // Check if the GPIO is already exported  
    if (access(gpio_path, F_OK) != 0)  
    {  
        // Construct the command to export the GPIO  
        snprintf(export_cmd, sizeof(export_cmd), "echo %d > /sys/class/gpio/export", rs485_gpio_number);  
  
        // Execute the command to export the GPIO  
        system(export_cmd);  
    }  
}  
  


void set485TX()
{
    system("echo out > /sys/class/gpio/gpio428/direction");
    system("echo 1 > /sys/class/gpio/gpio428/value");
}

void set485RX()
{
    system("echo out > /sys/class/gpio/gpio428/direction");
    system("echo 0 > /sys/class/gpio/gpio428/value");
}

int set_rs485(int fd)
{
	struct serial_rs485 rs485conf;

	/* Enable RS485 mode: */
	rs485conf.flags |= SER_RS485_ENABLED;
#if 0
	/* Set logical level for RTS pin equal to 1 when sending: */
	rs485conf.flags |= SER_RS485_RTS_ON_SEND;
	/* or, set logical level for RTS pin equal to 0 when sending: */
	//rs485conf.flags &= ~(SER_RS485_RTS_ON_SEND);
	/* Set logical level for RTS pin equal to 1 after sending: */
	//rs485conf.flags |= SER_RS485_RTS_AFTER_SEND;
	/* or, set logical level for RTS pin equal to 0 after sending: */
	rs485conf.flags &= ~(SER_RS485_RTS_AFTER_SEND);
	/* Set this flag if you want to receive data even whilst sending data */
	//rs485conf.flags |= SER_RS485_RX_DURING_TX;
#endif

	if (ioctl (fd, TIOCSRS485, &rs485conf) < 0)
	{
		/* Error handling. See errno. */
		perror("error ioctl set rs485!\n");
        printf("ERRNO: %d\n", errno);
		return -1;
	}
	return 0;
}

int openPortRaw(const char* port, int openMode)
{
    int fd;
    if(port == NULL)
    {
        printf("ERROR: Port device error!\n");
        return _ERROR;
    }
    fd = open(port, openMode);
    if(fd < 0)
    {
        printf("ERROR: Open port failed!\n");
        return _ERROR;
    }
    return fd;
}

int setPortAttr(int fd, struct termios* portAttr)
{
	if(portAttr == NULL)
    {
        printf("ERROR: Port params struct(input) is null!\n");
        return _ERROR;
    }

	portAttr->c_cflag |= CLOCAL | CREAD;
	portAttr->c_iflag &= ~(BRKINT | ICRNL | IGNCR | ISTRIP | IXON);
	portAttr->c_oflag &= ~(OPOST);
	portAttr->c_lflag &= ~(ICANON | ISIG | ECHO | ECHOE);
	portAttr->c_cc[VTIME] = 5;
	portAttr->c_cc[VMIN] = 0;

    tcflush(fd,TCIOFLUSH);

	return _SUCCESS;
}

int setBaudrate(int fd, struct termios* portAttr, int baudrate)
{
    int i;
    if(portAttr == NULL)
    {
        printf("ERROR: Port params struct(input) is null!\n");
        return _ERROR;
    }
    for( i= 0; i < sizeof(speed_arr) / sizeof(int); i++)
    {
        if(baudrate == name_arr[i])
        {
            tcflush(fd, TCIOFLUSH);
            cfsetispeed(portAttr, speed_arr[i]);
            cfsetospeed(portAttr, speed_arr[i]);
            tcflush(fd,TCIOFLUSH);
            break;
        }
    }
    return _SUCCESS;
}

int setDatabits(struct termios* portAttr, int databits)
{
    if(portAttr == NULL)
    {
        printf("ERROR: Port params struct(input) is null!\n");
        return _ERROR;
    }
    portAttr->c_cflag &= ~CSIZE;
	switch (databits)
	{
        case 7:
            portAttr->c_cflag |= CS7;
            break;
        case 8:
            portAttr->c_cflag |= CS8;
            break;
        default:
            printf("Unsupported data size!\n");
            return _ERROR;
	}
	return _SUCCESS;
}

int setParity(struct termios* portAttr, unsigned char paritybits)
{
    if(portAttr == NULL)
    {
        printf("ERROR: Port params struct(input) is null!\n");
        return _ERROR;
    }
    switch (paritybits)
	{
		case 'n':
		case 'N':
			portAttr->c_cflag &= ~PARENB;   /* Clear parity enable */
			portAttr->c_iflag &= ~INPCK;     /* Enable parity checking */
			break;
		case 'o':
		case 'O':
			portAttr->c_cflag |= (PARODD | PARENB);
			portAttr->c_iflag |= INPCK;             /* Disnable parity checking */
			break;
		case 'e':
		case 'E':
			portAttr->c_cflag |= PARENB;     /* Enable parity */
			portAttr->c_cflag &= ~PARODD;   
			portAttr->c_iflag |= INPCK;       /* Disnable parity checking */
			break;
		default:
			printf("Unsupported parity!\n");
			return _ERROR;
    }
    return _SUCCESS;
}

int setStopbits(struct termios* portAttr, int stopbits)
{
    if(portAttr == NULL)
    {
        printf("ERROR: Port params struct(input) is null!\n");
        return _ERROR;
    }
    switch(stopbits)
	{
		case 1:
			portAttr->c_cflag &= ~CSTOPB;
			break;
		case 2:
			portAttr->c_cflag |= CSTOPB;
		   break;
		default:
			 printf("Unsupported stop bits!\n");
			 return _ERROR;
	}
	return _SUCCESS;
}

int open_port(const char* port, int openMode, SerialPortParams params)
{
    int fd;
    struct termios options;
    printf("Open Port...\n");
    switch(openMode)
    {
    case 1:
        fd = openPortRaw(port, O_RDWR | O_NOCTTY | O_NONBLOCK);
        break;
    default:
        fd = openPortRaw(port, O_RDWR | O_NOCTTY);
        break;
    }
    if(fd < 0)
    {
        return _ERROR;
    }
    //if(set_rs485(fd) < 0)
    //{
    //    close(fd);
    //    return _ERROR;
    //}

    printf("Set attribute...\n");
    if(tcgetattr(fd, &options) != 0)
    {
    	close(fd);
		printf("ERROR: Get Serial port params failed!\n");
		return _ERROR;
	}
	if(setPortAttr(fd, &options) < 0)
    {
		close(fd);
        return _ERROR;
    }
	if(setBaudrate(fd, &options, params.baudrate) < 0)
    {
		close(fd);
        return _ERROR;
    }
    if(setDatabits(&options, params.databits) < 0)
    {
    	close(fd);
        return _ERROR;
    }
    if(setParity(&options, params.paritybits) < 0)
    {
    	close(fd);
        return _ERROR;
    }
    if(setStopbits(&options, params.stopbits) < 0)
    {
    	close(fd);
        return _ERROR;
    }
    printf("Save attribute...\n");
    if(tcsetattr(fd,TCSANOW,&options) != 0)
	{
    	close(fd);
		printf("ERROR: Set Serial port params failed!\n");
		return _ERROR;
	}
	printf("INFO: Save attribute successful!\n");
	return fd;
}

int close_port(int fd)
{
    if(close(fd) < 0)
    {
        printf("ERROR: Close port error!\n");
        return _ERROR;
    }
    return _SUCCESS;
}

/**
 * Writing data to a serial port.
 * 
 * @param fd The file descriptor of a serial port.
 * @param msg The buffer zone where the data to be written is stored.
 * @param length The length of the data to be written.
 * @return The actual number of bytes written, or -1 if an error occurs.
 * @usage 
 * #define MAXLEN 128
 * send_packet_rs485(fd, "eth1 is good", MAXLEN);  
 */
int send_packet_rs485(int fd, const char *msg, unsigned int length)
{
    //exportGPIO();
    //set485TX();
    
	printf("fd:%d msg_len:%d msg:%s\n",fd,length,msg);
    /* check msg is null or not */
	if (msg == NULL) {
        printf("ERROR: Buffer is NULL!\n");
        return _ERROR;
    }

    /* check length is 0 or not */
    if (length == 0) {
        printf("INFO: Length of data to write is 0.\n");
        return 0;
    }

    int bytesWrittenTotal = 0;  //total bytes
    int bytesWrittenNow = 0;    //now bytes

    while (bytesWrittenTotal < length) {
        /* write msg */
        bytesWrittenNow = write(fd, msg + bytesWrittenTotal, length - bytesWrittenTotal);

        /* if error */
        if (bytesWrittenNow < 0) {
            perror("ERROR: Failed to write data to serial port");
            return _ERROR;
        }
        
        bytesWrittenTotal += bytesWrittenNow;
    }
    return bytesWrittenTotal;
}

int receive_packet_rs485(int fd, unsigned char *msg, unsigned int length, int wait_time)
{
    //exportGPIO();
    //set485RX();
    
    /* check msg is null or not */
    if (msg == NULL) {
        printf("ERROR: Buffer is NULL!\n");
        return _ERROR;
    }

    /* check length is 0 or not */
    if (length == 0) {
        printf("INFO: Length of data to read is 0. Nothing to do.\n");
        return 0;
    }

    /* clear the msg */
    memset(msg, 0, length);
	

    int bytesReadTotal = 0;    // The total number of bytes read so far.
    int bytesReadNow = 0;      // The number of bytes read during this call to read.
    int isDataReceived = 0;    // A flag used to detect whether data reception has already started.

    /* Set up the file descriptor set for select(). */
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(fd, &readfds);

    /* Set up the timeout structure. */
    struct timeval timeout;
    timeout.tv_sec = wait_time;
    timeout.tv_usec = 0;

	//printf("fd:%d msg_len:%d msg:%s wait_time:%d\n",fd,length,msg,wait_time);

    while (bytesReadTotal < length) {
        /* Try to read data. */
        int retval = select(fd + 1, &readfds, NULL, NULL, &timeout);
        /* If select() times out, return the current number of bytes read. */
        if (retval == 0) {
            break;
        }
        /* If select() encounters an error, print an error message and return. */
        if (retval == -1) {
            perror("ERROR: select() failed");
            return _ERROR;
        }
        bytesReadNow = read(fd, msg + bytesReadTotal, length - bytesReadTotal);

        /* if error */
        if (bytesReadNow < 0) {
            perror("ERROR: Failed to read data from serial port");
            return _ERROR;
        }
        /* If no bytes have been read and data reception has not yet started, continue */
        if ((bytesReadNow == 0) && !isDataReceived) {
            continue;
        }
        /* If any bytes are read, update the flag to indicate that data reception has begun. */
        if (bytesReadNow > 0) {
            isDataReceived = 1;
        }
        /* Upon reaching the end of the file, exit */
        if ((bytesReadNow == 0) && isDataReceived) {
            break;
        }
        /* Update the total number of bytes read so far. */
        bytesReadTotal += bytesReadNow;
    }
	if(bytesReadTotal==0) /*if timeout with retval==0*/
	{
		printf("Timeout!\n");
		return _ERROR;
	}
	printf("fd:%d msg_len:%d msg:%s\n",fd,length,msg);
    return bytesReadTotal;
}



