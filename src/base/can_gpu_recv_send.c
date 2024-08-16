#include "can_gpu_recv_send.h"
unsigned char can0RecvArray[1024]={0};
unsigned char can1RecvArray[1024]={0};

int isAllZero(char arr[], int size) {  
    for (int i = 0; i < size; i++) {  
        if (arr[i] != 0) {  
            return 0;  
        }  
    }  
    return 1;  
}  

int send_packet_can_gpu(int can_id, const char *msg, int len)
{
    int sockfd;
    struct sockaddr_can addr;
    struct can_frame frame;
    struct ifreq ifr;

    sockfd = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (sockfd < 0) {
        perror("Socket creation failed");
        goto error;
    }

    can_id == 0 ? strncpy(ifr.ifr_name, NAME0, strlen(NAME0) + 1) : 
                    strncpy(ifr.ifr_name, NAME1, strlen(NAME1) + 1);
    if (ioctl(sockfd, SIOCGIFINDEX, &ifr) < 0) {
        perror("IOCTL failed");
        goto error;
    }

    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("Bind failed");
        goto error;
    }

    memset(&frame, 0, sizeof(frame));
    frame.can_id = CAN_ID; 
    frame.can_dlc = len;
    memcpy(frame.data, msg, frame.can_dlc);

    if (write(sockfd, &frame, sizeof(struct can_frame)) != sizeof(struct can_frame)) 
    {
        perror("Write failed");
        goto error;
    }
    close(sockfd);
    return _SUCCESS;
error:
    // 错误处理，确保关闭套接字
    if (sockfd >= 0) {
        close(sockfd);
    }
    return _ERROR;
}

int receive_packet_can_gpu(int can_id, unsigned char *msg, int length, long wait_time)
{
    int len=-1;
	if(length > MAX_CAN_DATA_LENGTH)
		len = MAX_CAN_DATA_LENGTH;
	else
		len = length;
    
	char data_frame[RECEIVED_CAN_DATA_PACKAGE_SIZE];
    if (can_id == 0)
    {
        if (can0Recv(len, wait_time))
        {
            perror("Receive can0 failed");
            return _ERROR;
        }
        memcpy(data_frame, can0RecvArray, len);
        memset(can0RecvArray, 0x0, sizeof(can0RecvArray));
    }
    else
    {
        if (can1Recv(len, wait_time))
        {
            perror("Receive can1 failed");
            return _ERROR;
        }
        memcpy(data_frame, can1RecvArray, len);
        memset(can1RecvArray, 0x0, sizeof(can1RecvArray));
    }

    if (isAllZero(data_frame,RECEIVED_CAN_DATA_PACKAGE_SIZE)) 
    { 
        perror("Receive data 0 byte\n");
    }
    memcpy(msg, data_frame, len);
    return _SUCCESS;  
}


int can0Recv(int len, long wait_time)
{
    int sockfd;
    struct sockaddr_can addr;
    struct can_frame frame;
    struct ifreq ifr;

    sockfd = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (sockfd < 0) {
        perror("Socket creation failed");
        goto error;
    }

    strncpy(ifr.ifr_name, "can0", strlen(NAME1) + 1);
    if (ioctl(sockfd, SIOCGIFINDEX, &ifr) < 0) {
        perror("IOCTL failed");
        goto error;
    }

    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("Bind failed");
        goto error;
    }

    memset(&frame, 0, sizeof(frame));

    struct timeval timeout;
    fd_set readSet;
    int ret;

    timeout.tv_sec = wait_time;
    timeout.tv_usec = 0;

    FD_ZERO(&readSet);
    FD_SET(sockfd, &readSet);

    ret = select(sockfd + 1, &readSet, NULL, NULL, &timeout);
    if (ret < 0) {
        perror("Error in select");
        goto error;
    } 
    else if (ret == 0) 
    {
        printf("Timeout occurred, no data received.\n");
        goto error;
    } 
    else 
    {
        ssize_t nbytes = read(sockfd, &frame, sizeof(struct can_frame));
        if (nbytes < 0) {
            perror("Read error");
            goto error;
        }
        memcpy(can0RecvArray, frame.data, sizeof(frame.data)); 
    }
    close(sockfd);
    return _SUCCESS;
error:
    // 错误处理，确保关闭套接字
    if (sockfd >= 0) {
        close(sockfd);
    }
    return _ERROR;
}

int can1Recv(int len, long wait_time)
{
    int sockfd;
    struct sockaddr_can addr;
    struct can_frame frame;
    struct ifreq ifr;

    sockfd = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (sockfd < 0) {
        perror("Socket creation failed");
        goto error;
    }

    strncpy(ifr.ifr_name, "can1", strlen(NAME1) + 1);
    if (ioctl(sockfd, SIOCGIFINDEX, &ifr) < 0) {
        perror("IOCTL failed");
        goto error;
    }

    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("Bind failed");
        goto error;
    }

    memset(&frame, 0, sizeof(frame));

    struct timeval timeout;
    fd_set readSet;
    int ret;

    timeout.tv_sec = wait_time;
    timeout.tv_usec = 0;

    FD_ZERO(&readSet);
    FD_SET(sockfd, &readSet);

    ret = select(sockfd + 1, &readSet, NULL, NULL, &timeout);
    if (ret < 0) {
        perror("Error in select");
        goto error;
    } 
    else if (ret == 0) 
    {
        printf("Timeout occurred, no data received.\n");
        goto error;
    } 
    else 
    {
        ssize_t nbytes = read(sockfd, &frame, sizeof(struct can_frame));
        if (nbytes < 0) {
            perror("Read error");
            goto error;
        }
        memcpy(can1RecvArray, frame.data, sizeof(frame.data)); 
    }
    close(sockfd);
    return _SUCCESS;
error:
    // 错误处理，确保关闭套接字
    if (sockfd >= 0) {
        close(sockfd);
    }
    return _ERROR;
}

