
#include "initializer_closer_lib.h"

pthread_mutex_t lock;

// 在程序初始化时创建锁  
void initialize_lock() {  
    pthread_mutex_init(&lock, NULL);  
}  
  
// 在程序结束时销毁锁  
void destroy_lock() {  
    pthread_mutex_destroy(&lock);  
}



int initializer_transfer(char *initializer_name, const char *interface_name)
{
	pthread_mutex_lock(&lock); // 在函数开始时加锁

	int i;  
    int found = 0;  
    for (i = 0; i < initializer_num; i++) {  
        if (strcmp(initializer_name, initializer_name_array[i]) == 0) {  
            found = 1;  
            break;  
        }  
    }  
    if (!found) {  
		pthread_mutex_unlock(&lock); // 在返回前解锁  
        return _ERROR;  
    }  
  
    if (strcmp(initializer_name, "eth_initializer_normal") == 0) {
		int ret = eth_initializer_normal(interface_name);
		pthread_mutex_unlock(&lock); // 在返回前解锁  
        return ret;  
    }  
    if (strcmp(initializer_name, "rs485_initializer_normal") == 0) { 
		int ret = rs485_initializer_normal(interface_name);
		pthread_mutex_unlock(&lock); // 在返回前解锁  
        return ret;  
    }  
    if (strcmp(initializer_name, "can_fpu_initializer_normal") == 0) { 
		int ret = can_fpu_initializer_normal(interface_name);  
		pthread_mutex_unlock(&lock); // 在返回前解锁  
        return ret;  
    }  
  	pthread_mutex_unlock(&lock); // 在返回前解锁  
    return _ERROR; // This should never be reached, but kept for safety.  
}


int closer_transfer(char *closer_name, const char *interface_name)
{
	pthread_mutex_lock(&lock); // 在函数开始时加锁

	int i;  
    int found = 0;  
    for (i = 0; i < closer_num; i++) {  
        if (strcmp(closer_name, closer_name_array[i]) == 0) {  
            found = 1;  
            break;  
        }  
    }  
    if (!found) {  
		pthread_mutex_unlock(&lock); // 在返回前解锁  
        return _ERROR;  
    }  
  
    if (strcmp(closer_name, "eth_closer_normal") == 0) {
		int ret = eth_closer_normal(interface_name);
		pthread_mutex_unlock(&lock); // 在返回前解锁  
        return ret;  
    }  
    if (strcmp(closer_name, "rs485_closer_normal") == 0) {  
		int ret = rs485_closer_normal(interface_name); 
		pthread_mutex_unlock(&lock); // 在返回前解锁  
        return ret;  
    }  
    if (strcmp(closer_name, "can_fpu_closer_normal") == 0) {
		int ret = can_fpu_closer_normal(interface_name); 
		pthread_mutex_unlock(&lock); // 在返回前解锁  
        return ret;  
    }  
  	pthread_mutex_unlock(&lock); // 在返回前解锁  
    return _ERROR; // This should never be reached, but kept for safety.  
}


int eth_initializer_normal(const char *interface_name)
{
	int i = get_interface_index(interface_name);
	int initialized_flag = get_initialized_flag_by_index(i);
	if(initialized_flag>0)
	{
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
		}
	}
	return _SUCCESS;

}

int rs485_initializer_normal(const char *interface_name)
{
	int i = get_interface_index(interface_name);
	int initialized_flag = get_initialized_flag_by_index(i);
	if(initialized_flag>0)
	{
		return _SUCCESS;
	}
	
	SerialPortParams params;
	params.baudrate = get_rs485_baud_rate_by_index(i);
	params.databits = get_databits_by_index(i);
	params.stopbits = get_stopbits_by_index(i);
	params.paritybits = get_paritybits_by_index(i);
				
	int fd = open_port(interface_name, 0, params);
	if(fd < 0)
	{
		printf("Open port failed!\n");
		return _ERROR;
	}
	set_temporary_fd(i,fd);
	
	printf("interface_name:%s, fd:%d, baudrate:%d, databits:%d, stopbits:%d, paritybits:%c\n",interface_name,fd,
				params.baudrate,params.databits,params.stopbits,params.paritybits);

	char *rs485_dev_path = get_rs485_dev_path_by_index(i);
	for(int j=0;j<get_interface_cnt();j++)
	{
		if(strcmp(get_interface_type_by_index(j),"rs485")==0 && strcmp(rs485_dev_path,get_rs485_dev_path_by_index(j))==0)
		{
			set_initialized_flag_by_index(j,1);
		}
	}
	return _SUCCESS;
}


int can_fpu_initializer_normal(const char *interface_name)
{
	int i = get_interface_index(interface_name);
	int initialized_flag = get_initialized_flag_by_index(i);
	if(initialized_flag>0)
	{
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

int eth_closer_normal(const char *interface_name)
{
	int i = get_interface_index(interface_name);
	int initialized_flag = get_initialized_flag_by_index(i);
	if(initialized_flag<0)
	{
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
	int i = get_interface_index(interface_name);
	int initialized_flag = get_initialized_flag_by_index(i);
	if(initialized_flag<0)
	{
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
		}
	}
	return _SUCCESS;
}

int can_fpu_closer_normal(const char *interface_name)
{
	int i = get_interface_index(interface_name);
	int initialized_flag = get_initialized_flag_by_index(i);
	if(initialized_flag<0)
	{
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





