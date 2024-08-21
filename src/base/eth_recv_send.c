
#include "eth_recv_send.h"



// 在程序初始化时创建锁  
void initialize_eth_lock() {  
    pthread_mutex_init(&eth_lock, NULL);  
}  
  
// 在程序结束时销毁锁  
void destroy_eth_lock() {  
    pthread_mutex_destroy(&eth_lock);  
}



/**  
 * Function to receive a packet from a specified network interface_name and process it.  
 * Specifically, it listens for SNMP packets and copies the payload into the provided buffer.  
 *  
 * @param interface_name A pointer to a string containing the name of the network interface_name to listen on.  
 * @param msg A pointer to an unsigned char array where the payload of the received SNMP packet will be copied. 
 * @usage 
 * unsigned char msg[100];
 * receive_packet("eth1", msg);  
 */  
int receive_packet(int sockfd, unsigned char *msg,long max_waiting_time)  
{  

	if(msg==NULL)
	{
		printf("[ERROR] receive_packet got a NULL msg!\n");
		return _ERROR;
	}

	
	//printf("source_interface: %s\n",interface_name);


    int ret = 0;
    fd_set readfd;
    struct timeval timeout;
    timeout.tv_sec  = max_waiting_time; 
    timeout.tv_usec = 0; 
    // Wait some secs to receive packets
    FD_ZERO(&readfd); 
    FD_SET(sockfd, &readfd);
	
    ret = select(sockfd + 1, &readfd, NULL, NULL, &timeout);
	  
    if (ret == -1)
    {
        perror("select");
		
        return _ERROR;
    }
    else if (ret == 0)
    {
        printf("No data was received.\n");
		
        return _ERROR;
    }
    else
    {
	        if (FD_ISSET(sockfd, &readfd))
	        {
	            unsigned char packet[65535];// Buffer to hold the received packet  
	            int packet_len = recvfrom(sockfd, packet, sizeof(packet), 0, NULL, NULL);
	            // If receive fails, print error and continue 
	            if (packet_len < 0) {
	                perror("recvfrom");
					
	                return _ERROR;
	            }
	            struct ether_header *eh = (struct ether_header *)packet;
	            // Check if the received packet is an SNMP packet
	            if (ntohs(eh->ether_type) == ETH_P_SNMP)
	            {
	                // Calculate the length of the payload and copy it to the provided buffer
	                char payload[packet_len - sizeof(struct ether_header)];
	                memcpy(payload, packet + sizeof(struct ether_header), packet_len - sizeof(struct ether_header));
	                // Copy the payload into the msg buffer, ensuring not to overflow  
	                // Note: This is a potential issue as strlen(msg) might not be what is intended here.  
	                // It's safer to use a fixed size or ensure msg is null-terminated and use strncpy with a safe length.  
	                // For simplicity, assuming msg is large enough and null-terminated.  
					//printf("%s %d\n",payload,strlen(payload)); 
					strncpy(msg, payload, strlen(payload) + 1);  
					//printf("%s %d\n",msg,strlen(msg));
	            }
	        }
    } 
    
	return _SUCCESS;

}





unsigned char srcmac[6];
unsigned char dstmac[6];


int stringToMacAddress(const char* macStr, unsigned char* macAddr) {  
    if (macStr == NULL || macAddr == NULL) {  
        return -1; // 无效的输入参数  
    }  
  
    int i = 0;  
    char hexByte[3]; // 临时存储两个十六进制字符  
  
    // 遍历字符串，每次处理两个十六进制字符  
    for (int j = 0; i < 6 && macStr[j] != '\0'; j += 3) {  
        // 跳过':'字符（如果存在）  
        if (macStr[j] == ':')  
            j++;  
  
        // 复制两个十六进制字符到临时数组  
        strncpy(hexByte, macStr + j, 2);  
        hexByte[2] = '\0'; // 确保字符串以null结尾  
  
        // 将十六进制字符串转换为整数，并存储在macAddr数组中  
        char* end;  
        long int hexValue = strtol(hexByte, &end, 16);  
        if (hexValue < 0 || hexValue > 0xFF || end != hexByte + 2) {  
            // 转换失败或超出范围  
            return -1;  
        }  
        macAddr[i++] = (unsigned char)hexValue;  
    }  
  
    // 如果未处理足够的字节，则返回错误  
    if (i != 6) {  
        return -1;  
    }  
  
    return 0; // 转换成功  
}


/**  
 * Sends a packet over a specified network interface_name containing a given message.  
 * The packet is constructed with an Ethernet header and the provided message as payload.  
 *  
 * @param interface_name The name of the network interface_name to send the packet through.  
 * @param message   The message to be sent as the payload of the packet.  
 * @usage send_packet("eth1", "eth1 is good", "\x3A\x0F\x58\xF4\x95\x89", "\x20\x7B\xD2\x3C\xF2\x9D");  
 */  
int send_packet(int sockfd, struct sockaddr_ll* sock_addr_value_addr, const char *message, const char *ether_shost, const char *ether_dhost)  
{  

	if(message==NULL)
	{
		printf("[ERROR] receive_packet got a NULL msg!\n");
		return _ERROR;
	}

	if(!ether_shost)
	{
		printf("[ERROR] receive_packet got a NULL ether_shost!");
		return _ERROR;
	}else if(!ether_dhost)
	{
		printf("[ERROR] receive_packet got a NULL ether_dhost!");
		return _ERROR;
	}

	/*if(!ether_shost)
	{
		printf("ether_shost is NULL!");
	}else if(!ether_dhost)
	{
		printf("ether_dhost is NULL!");
	}
	printf("source_interface:%s ether_shost:%s ether_dhost:%s\n",interface_name,ether_shost,ether_dhost);*/


	stringToMacAddress(ether_shost,srcmac);
	stringToMacAddress(ether_dhost,dstmac);

	
    // Calculate the length of the message  
    int message_len = strlen(message);  
  
    // Allocate memory for the packet, including the size of the Ethernet header and the message length  
    unsigned char packet[sizeof(struct ether_header) + message_len];  
  
    // Initialize the Ethernet header pointer to the start of the packet buffer  
    struct ether_header *eh = (struct ether_header *)packet;  
  
    // Set the Ethernet type field to SNMP  
    eh->ether_type = htons(ETH_P_SNMP);  
  
    // Copy the destination and source MAC addresses into the Ethernet header  
    memcpy(eh->ether_dhost, dstmac, ETH_ALEN);  
    memcpy(eh->ether_shost, srcmac, ETH_ALEN);  
  
    // Copy the message payload into the packet, immediately after the Ethernet header  
    memcpy(packet + sizeof(struct ether_header), message, message_len);  
  
    
  
    // Send the packet  
    if (sendto(sockfd, packet, sizeof(packet), 0, (struct sockaddr *)sock_addr_value_addr, sizeof(*sock_addr_value_addr)) < 0) {  
        // Error handling: print error message  
        perror("sendto");  
		close(sockfd); 
		return _ERROR;
    }  
  
    // Close the socket  
    close(sockfd); 
	return _SUCCESS;

}

