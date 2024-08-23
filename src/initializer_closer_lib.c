
#include "initializer_closer_lib.h"

pthread_mutex_t initializer_lock;

// 在程序初始化时创建锁  
void initialize_initializer_lock() {  
    pthread_mutex_init(&initializer_lock, NULL);  
}  
  
// 在程序结束时销毁锁  
void destroy_initializer_lock() {  
    pthread_mutex_destroy(&initializer_lock);  
}

 

int initializer_transfer(char *initializer_name, const char *interface_name) 
{
	pthread_mutex_lock(&initializer_lock); // 在函数开始时加锁

	printf("***********************************wait for %s being initialized\n",interface_name);
	int i;  
    int found = 0;  
    for (i = 0; i < initializer_num; i++) {  
        if (strcmp(initializer_name, initializer_name_array[i]) == 0) {  
            found = 1;  
            break;  
        }  
    }  
    if (!found) {  
		pthread_mutex_unlock(&initializer_lock); // 在返回前解锁  
		printf("***********************************%s failed to be initialized because its initializer not found\n",interface_name);
        return _ERROR;  
    }  
  
    if (strcmp(initializer_name, "eth_initializer_normal") == 0) {
		int ret = eth_initializer_normal(interface_name);
		printf("***********************************%s initialized done\n",interface_name);
		pthread_mutex_unlock(&initializer_lock); // 在返回前解锁  
        return ret;  
    }  

    if (strcmp(initializer_name, "eth_initializer_xy") == 0) {
		int ret = eth_initializer_xy(interface_name);
		printf("***********************************%s initialized done\n",interface_name);
		pthread_mutex_unlock(&initializer_lock); // 在返回前解锁  
        return ret;  
    }  

	
    if (strcmp(initializer_name, "rs485_initializer_normal") == 0) { 
		int ret = rs485_initializer_normal(interface_name);
		printf("***********************************%s initialized done\n",interface_name);
		pthread_mutex_unlock(&initializer_lock); // 在返回前解锁  
        return ret;  
    }  
    if (strcmp(initializer_name, "can_fpu_initializer_normal") == 0) { 
		int ret = can_fpu_initializer_normal(interface_name);  
		printf("***********************************%s initialized done\n",interface_name);
		pthread_mutex_unlock(&initializer_lock); // 在返回前解锁  
        return ret;  
    }  
	if (strcmp(initializer_name, "can_gpu_initializer_normal") == 0) { 
		int ret = can_gpu_initializer_normal(interface_name);
		printf("***********************************%s initialized done\n",interface_name);
		pthread_mutex_unlock(&initializer_lock); // 在返回前解锁  
        return ret;  
    }  

	printf("***********************************%s failed to be initialized\n",interface_name);
  	pthread_mutex_unlock(&initializer_lock); // 在返回前解锁  
    return _ERROR; // This should never be reached, but kept for safety.  
}


int closer_transfer(char *closer_name, const char *interface_name)
{
	pthread_mutex_lock(&initializer_lock); // 在函数开始时加锁

	printf("***********************************wait for %s being closed\n",interface_name);
	int i;  
    int found = 0;  
    for (i = 0; i < closer_num; i++) {  
        if (strcmp(closer_name, closer_name_array[i]) == 0) {  
            found = 1;  
            break;  
        }  
    }  
    if (!found) {  
		pthread_mutex_unlock(&initializer_lock); // 在返回前解锁  
		printf("***********************************%s failed to be closed because its closer not found\n",interface_name);
        return _ERROR;  
    }  
  
    if (strcmp(closer_name, "eth_closer_normal") == 0) {
		int ret = eth_closer_normal(interface_name);
		printf("***********************************%s closed done\n",interface_name);
		pthread_mutex_unlock(&initializer_lock); // 在返回前解锁  
        return ret;  
    }  

    if (strcmp(closer_name, "eth_closer_xy") == 0) {
		int ret = eth_closer_xy(interface_name);
		printf("***********************************%s closed done\n",interface_name);
		pthread_mutex_unlock(&initializer_lock); // 在返回前解锁  
        return ret;  
    }  

	
    if (strcmp(closer_name, "rs485_closer_normal") == 0) {  
		int ret = rs485_closer_normal(interface_name); 
		printf("***********************************%s closed done\n",interface_name);
		pthread_mutex_unlock(&initializer_lock); // 在返回前解锁  
        return ret;  
    }  
    if (strcmp(closer_name, "can_fpu_closer_normal") == 0) {
		int ret = can_fpu_closer_normal(interface_name); 
		printf("***********************************%s closed done\n",interface_name);
		pthread_mutex_unlock(&initializer_lock); // 在返回前解锁  
        return ret;  
    }  
    if (strcmp(closer_name, "can_gpu_closer_normal") == 0) {
		int ret = can_gpu_closer_normal(interface_name); 
		printf("***********************************%s closed done\n",interface_name);
		pthread_mutex_unlock(&initializer_lock); // 在返回前解锁  
        return ret;  
    }  

	printf("***********************************%s failed to be closed\n",interface_name);
  	pthread_mutex_unlock(&initializer_lock); // 在返回前解锁  
    return _ERROR; // This should never be reached, but kept for safety.  
}


int eth_initializer_normal(const char *interface_name)
{
	initialize_eth_lock();

	int i = get_interface_index(interface_name);
	int initialized_flag = get_initialized_flag_by_index(i);
	if(initialized_flag>0)
	{
		printf("%s already initialized before\n",interface_name);
		return _SUCCESS;
	}
	
	char* ip_addr = get_ip_addr_by_index(i);  
	char* mask = get_net_mask_by_index(i);	
	char* ip_name = get_ip_name_by_index(i);	
			  
	if (ip_addr == NULL || mask == NULL) {	
		// Handle NULL pointers appropriately  
		return _ERROR; // Or some other error code	
	}  
	
	set_interface_status(interface_name,"sending_and_receiving");

	
	  
	// Use snprintf or similar to safely format strings for system call  
	char cmd_up[256];  
	snprintf(cmd_up, sizeof(cmd_up), "ifconfig %s up", ip_name);  
	system(cmd_up); 
			
	  
	char cmd_addr[256];  
	snprintf(cmd_addr, sizeof(cmd_addr), "ifconfig %s %s netmask %s", ip_name, ip_addr, mask);  
	system(cmd_addr);  
	
	printf("ip_name:%s, ip_addr:%s, mask:%s\n",ip_name,ip_addr,mask);

	for(int j=0;j<get_interface_cnt();j++)
	{
		if(strcmp(get_interface_type_by_index(j),"eth")==0 && strcmp(ip_name,get_ip_name_by_index(j))==0)
		{
			set_initialized_flag_by_index(j,1);
			//printf("aaa %s\n",*sock_addr_value_addr);
		}
	}
	
	return _SUCCESS;
}


int eth_initializer_xy(const char *interface_name)
{
	initialize_eth_lock();

	int i = get_interface_index(interface_name);
	int initialized_flag = get_initialized_flag_by_index(i);
	if(initialized_flag>0)
	{
		printf("%s already initialized before\n",interface_name);
		return _SUCCESS;
	}
	
	char* ip_addr = get_ip_addr_by_index(i);  
	char* mask = get_net_mask_by_index(i);	
	char* ip_name = get_ip_name_by_index(i);	
			  
	if (ip_addr == NULL || mask == NULL) {	
		// Handle NULL pointers appropriately  
		return _ERROR; // Or some other error code	
	}  
	
	set_interface_status(interface_name,"sending_and_receiving");

	
	  
	// Use snprintf or similar to safely format strings for system call  
	char cmd_up[256];  
	snprintf(cmd_up, sizeof(cmd_up), "ifconfig %s up", ip_name);  
	system(cmd_up); 
			
	  
	char cmd_addr[256];  
	snprintf(cmd_addr, sizeof(cmd_addr), "ifconfig %s %s netmask %s", ip_name, ip_addr, mask);  
	system(cmd_addr);  
	
	printf("ip_name:%s, ip_addr:%s, mask:%s\n",ip_name,ip_addr,mask);


	// Create a raw socket for packet capturing  
    int sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_SNMP));  
    if (sockfd < 0) {  
        // If socket creation fails, print error and return  
        perror("socket");  
        return _ERROR;  
    }  

    // Initialize a sockaddr_ll structure to bind the socket to a specific interface   
	struct sockaddr_ll* sock_addr_value_addr = get_sock_addr_value_addr(i);

	
    memset(sock_addr_value_addr, 0, sizeof(*sock_addr_value_addr)); // Clear the structure  
  
    sock_addr_value_addr->sll_family = AF_PACKET; // Set the address family  
    sock_addr_value_addr->sll_protocol = htons(ETH_P_SNMP); // Set the protocol to SNMP  
  
    // Use ifreq to get the index of the specified interface  
    struct ifreq ifr;  
    strncpy(ifr.ifr_name, ip_name, IFNAMSIZ); // Copy the interface name  
    if (ioctl(sockfd, SIOCGIFINDEX, &ifr) < 0) {  
        // If ioctl fails, print error, close socket, and return  
        perror("ioctl");  
        close(sockfd);  

		
        return _ERROR;  
    }  
  
    sock_addr_value_addr->sll_ifindex = ifr.ifr_ifindex; // Set the interface index  

	//printf("aaa %s\n",*sock_addr_value_addr);
  
    // Bind the socket to the specified interface  
    if (bind(sockfd, (struct sockaddr *)sock_addr_value_addr, sizeof(*sock_addr_value_addr)) < 0) {  
        // If bind fails, print error, close socket, and return  
        perror("bind");  
        close(sockfd);  
        return _ERROR;  
    }

	//printf("aaa %s\n",*sock_addr_value_addr);

	for(int j=0;j<get_interface_cnt();j++)
	{
		if(strcmp(get_interface_type_by_index(j),"eth")==0 && strcmp(ip_name,get_ip_name_by_index(j))==0)
		{
			set_initialized_flag_by_index(j,1);
			set_temporary_sockfd_by_index(j,sockfd);
			set_sock_addr_value_addr(j,sock_addr_value_addr);
			//printf("aaa %s\n",*sock_addr_value_addr);
		}
	}
	
	return _SUCCESS;

}


int rs485_initializer_normal(const char *interface_name)
{

	initialize_rs485_lock();

	int i = get_interface_index(interface_name);
	int initialized_flag = get_initialized_flag_by_index(i);
	if(initialized_flag>0)
	{
		printf("%s already initialized before\n",interface_name);
		return _SUCCESS;
	}
	
	SerialPortParams params;
	params.baudrate = get_rs485_baud_rate_by_index(i);
	params.databits = get_databits_by_index(i);
	params.stopbits = get_stopbits_by_index(i);
	params.paritybits = get_paritybits_by_index(i);


	char *rs485_dev_path = get_rs485_dev_path_by_index(i);	
	int fd = open_port(rs485_dev_path, 0, params);
	if(fd < 0)
	{
		printf("Open port failed!\n");
		return _ERROR;
	}
	//set_temporary_fd(i,fd);
	
	printf("interface_name:%s, fd:%d, baudrate:%d, databits:%d, stopbits:%d, paritybits:%c\n",interface_name,fd,
				params.baudrate,params.databits,params.stopbits,params.paritybits);


	for(int j=0;j<get_interface_cnt();j++)
	{
		if(strcmp(get_interface_type_by_index(j),"rs485")==0 && strcmp(rs485_dev_path,get_rs485_dev_path_by_index(j))==0)
		{
			set_initialized_flag_by_index(j,1);
			set_temporary_fd(j,fd);
		}
	}
	return _SUCCESS;
}


int can_fpu_initializer_normal(const char *interface_name)
{

	initialize_can_fpu_lock();

	int i = get_interface_index(interface_name);
	int initialized_flag = get_initialized_flag_by_index(i);
	if(initialized_flag>0)
	{
		printf("%s already initialized before\n",interface_name);
		return _SUCCESS;
	}

	
	int channel_id = get_channel_id_by_index(i);  
	int baud_rate = get_can_baud_rate_by_index(i);
	if(comCanSTDCfgInit(channel_id,baud_rate)<0)
	{
		set_interface_status(interface_name,"closed");
		return _ERROR;
	}
	set_interface_status(interface_name,"receiving");


	for(int j=0;j<get_interface_cnt();j++)
	{
		if(strcmp(get_interface_type_by_index(j),"can")==0 && channel_id == get_channel_id_by_index(j))
		{
			set_initialized_flag_by_index(j,1);
		}
	}
	return _SUCCESS;
}


int can_gpu_initializer_normal(const char *interface_name)
{
	initialize_can_gpu_lock();
	return _SUCCESS;
}





int eth_closer_xy(const char *interface_name)
{

	destroy_eth_lock();

	int i = get_interface_index(interface_name);
	int initialized_flag = get_initialized_flag_by_index(i);
	if(initialized_flag<0)
	{
		printf("%s already initialized before\n",interface_name);
		return _SUCCESS;
	}

	
	char* ip_name = get_ip_name_by_index(i);	
	
	char cmd[256];	
	snprintf(cmd, sizeof(cmd), "ifconfig %s down", ip_name);  
	system(cmd);

	// Close the socket  
    close(get_temporary_sockfd_by_index(i)); 

	for(int j=0;j<get_interface_cnt();j++)
	{
		if(strcmp(get_interface_type_by_index(j),"eth")==0 && strcmp(ip_name,get_ip_name_by_index(j))==0)
		{
			set_initialized_flag_by_index(j,-1);
			set_temporary_sockfd_by_index(j,-1);
		}
	}

	return _SUCCESS;
}



int eth_closer_normal(const char *interface_name)
{

	destroy_eth_lock();

	int i = get_interface_index(interface_name);
	int initialized_flag = get_initialized_flag_by_index(i);
	if(initialized_flag<0)
	{
		printf("%s already closed before\n",interface_name);
		return _SUCCESS;
	}

	
	char* ip_name = get_ip_name_by_index(i);	
	
	char cmd[256];	
	snprintf(cmd, sizeof(cmd), "ifconfig %s down", ip_name);  
	system(cmd);

	for(int j=0;j<get_interface_cnt();j++)
	{
		if(strcmp(get_interface_type_by_index(j),"eth")==0 && strcmp(ip_name,get_ip_name_by_index(j))==0)
		{
			set_initialized_flag_by_index(j,-1);
		}
	}
	
	return _SUCCESS;
}



int rs485_closer_normal(const char *interface_name)
{

	destroy_rs485_lock();

	int i = get_interface_index(interface_name);
	int initialized_flag = get_initialized_flag_by_index(i);
	if(initialized_flag<0)
	{
		printf("%s already closed before\n",interface_name);
		return _SUCCESS;
	}


	
	int fd = get_temporary_fd(i);
	close_port(fd);
	
	char *rs485_dev_path = get_rs485_dev_path_by_index(i);
	for(int j=0;j<get_interface_cnt();j++)
	{
		if(strcmp(get_interface_type_by_index(j),"rs485")==0 && strcmp(rs485_dev_path,get_rs485_dev_path_by_index(j))==0)
		{
			set_initialized_flag_by_index(j,-1);
			set_temporary_fd(j,-1);
		}
	}
	return _SUCCESS;
}

int can_fpu_closer_normal(const char *interface_name)
{

	destroy_can_fpu_lock();

	int i = get_interface_index(interface_name);
	int initialized_flag = get_initialized_flag_by_index(i);
	if(initialized_flag<0)
	{
		printf("%s already closed before\n",interface_name);
		return _SUCCESS;
	}
	

	int channel_id = get_channel_id_by_index(i);  
	for(int j=0;j<get_interface_cnt();j++)
	{
		if(strcmp(get_interface_type_by_index(j),"can")==0 && channel_id == get_channel_id_by_index(j))
		{
			set_initialized_flag_by_index(j,-1);
		}
	}
	return _SUCCESS;
}

int can_gpu_closer_normal(const char *interface_name)
{
	destroy_can_gpu_lock();
	return _SUCCESS;
}




