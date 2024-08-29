
#include "eth_recv_send_by_ip_addr.h"



void initialize_eth_by_ip_addr_lock() {  
    pthread_mutex_init(&eth_by_ip_addr_lock, NULL);  
}  

void destroy_eth_by_ip_addr_lock() {  
    pthread_mutex_destroy(&eth_by_ip_addr_lock);  
}



int send_data_tcp(const char* ip, int port, const char* data) {  
    int sock = 0;  
    struct sockaddr_in serv_addr;  
    char buffer[MAX_ETH_DATA_LENGTH] = {0};  
  
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {  
        printf("\n Socket creation error \n");  
        return _ERROR;  
    }  
  
    serv_addr.sin_family = AF_INET;  
    serv_addr.sin_port = htons(port);  
  
    // ��IPv4��ַ���ı�ת��Ϊ��������ʽ  
    if (inet_pton(AF_INET, ip, &serv_addr.sin_addr) <= 0) {  
        printf("\nInvalid address/ Address not supported \n"); 
		close(sock); // �ر��׽���  
        return _ERROR;  
    }  
  
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {  
        printf("\nConnection Failed \n");  
		close(sock); // �ر��׽���  
        return _ERROR;  
    }  
  
    send(sock, data, strlen(data), 0);  
    //printf("Data sent\n");  
  
    //int valread = read(sock, buffer, MAX_ETH_DATA_LENGTH);  
    //printf("Received: %s\n", buffer);  
  
    close(sock); // �ر��׽���  
    return _SUCCESS;  
}  



int receive_data_tcp(const char* ip, int port,char *buffer) {  
    int server_fd, new_socket;  
    struct sockaddr_in address;  
    int opt = 1;  
    int addrlen = sizeof(address);  

    // �����׽����ļ�������  
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {  
        perror("socket failed");  
        return _ERROR;  
    }  
  
    // ���׽��ֵ�ָ��IP�Ͷ˿�  
    address.sin_family = AF_INET;  
    address.sin_addr.s_addr = inet_addr(ip);  
    address.sin_port = htons(port);  
  
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {  
        perror("bind failed");  
        return _ERROR;  
    }  
  
    // ��ʼ�������ȴ��ͻ�������  
    if (listen(server_fd, 3) < 0) {  
        perror("listen");  
        return _ERROR;  
    }  
  
    // �������Կͻ��˵�����  
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {  
        perror("accept");  
        return _ERROR;   
    }  
  
    // ��ȡ�ͻ��˷��͵�����  
    int valread = read(new_socket, buffer, MAX_ETH_DATA_LENGTH);  
    //printf("Received: %s\n", buffer);  

  
    // �ر��׽���  
    close(new_socket);  
    close(server_fd); 
	return _SUCCESS;  
}  
  


  
// �������ݰ�  
int receive_packet_by_ip_addr(const char *listened_ip_addr,int listened_port_id, unsigned char *msg, long max_waiting_time) {  

    if (msg == NULL) {  
        printf("[ERROR] receive_packet got a NULL msg!\n");  
        return _ERROR;  
    }  

	return receive_data_tcp(listened_ip_addr,listened_port_id,msg);
	
  	/*
    char cmd[256];  
    snprintf(cmd, sizeof(cmd), "nc -l -p %d -w %ld", listened_port_id, max_waiting_time);  
    FILE *fp = popen(cmd, "r");  
    if (fp == NULL) {  
        printf("[ERROR] Failed to start nc for receiving.\n");  
        return _ERROR;  
    }  

    int i = 0;  
    while (i < length - 1 && fscanf(fp, "%c", &msg[i]) == 1) {
        i++;  
    }
	if (i < length) { 
		msg[i] = '\0';  
    }  
	pclose(fp);  
    return _SUCCESS;  */

}  
  
// �������ݰ�  
int send_packet_by_ip_addr(const char *target_ip, int target_port_id,const char *message) {  
    if (message == NULL) {  
        printf("[ERROR] receive_packet got a NULL message!\n");  
        return _ERROR;  
    }  

	return send_data_tcp(target_ip,target_port_id,message);
	/*
    char cmd[256];  
    snprintf(cmd, sizeof(cmd), "echo '%s' | nc %s %d", message, target_ip, target_port_id);  
    int result = system(cmd);  
    if (result != 0) {  
        printf("[ERROR] Failed to send data.\n");  
        return _ERROR;  
    }    
    return _SUCCESS;  */

}

