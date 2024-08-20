

#include "info_manager.h"

 
pthread_mutex_t communication_info_lock;

// 在程序初始化时创建锁  
void initialize_communication_info_lock() {  
    pthread_mutex_init(&communication_info_lock, NULL);  
}  
  
// 在程序结束时销毁锁  
void destroy_communication_info_lock() {  
    pthread_mutex_destroy(&communication_info_lock);  
}

// 将字符串转换为 time_t  
int string_to_time_t(const char* time_buffer, time_t* parsed_time) {  
    struct tm tm_parsed;  
    if (strptime(time_buffer, "%c", &tm_parsed) != NULL) {  
        *parsed_time = mktime(&tm_parsed);  
        if (*parsed_time != (time_t)-1) {  
            return _SUCCESS; // 成功  
        } else {  
            return _ERROR; // mktime 失败  
        }  
    } else {  
        return _ERROR; // strptime 失败  
    }  
 
}  
  
// 将 time_t 转换为字符串  
int time_t_to_string(time_t time_val, char* buffer, size_t buffer_size) {  
    struct tm* tm_info = localtime(&time_val);  
    if (tm_info == NULL) {  
        return _ERROR; // localtime 失败  
    }  
      
    if (strftime(buffer, buffer_size, "%c", tm_info) == 0) {  
        return _ERROR; // strftime 失败（通常是由于缓冲区太小）  
    }  
      
    return _SUCCESS; // 成功  
}  

int get_initialized_flag_by_index(int i)
{
	return interface_info_array[i].initialized_flag;
}

void set_initialized_flag_by_index(int i,int flag)
{
	interface_info_array[i].initialized_flag = flag;
}



int is_this_interface_in_current_node(const char* interface_name)
{
	for (size_t i = 0; i < interface_cnt; i++) {
        if (strcmp(interface_info_array[i].interface_name, interface_name) == 0) {
            return 1;
        }
    }
    return 0;
}

char* get_interface_name_by_linked_interface_name(char* linked_interface_name)
{
	for (size_t i = 0; i < interface_cnt; i++) {
        if (strcmp(interface_info_array[i].linked_interface_name, linked_interface_name) == 0) {
            return interface_info_array[i].interface_name;
        }
    }
    return NULL;
}




void write_communication_info_array_to_json(const char* filename)
{
	pthread_mutex_lock(&communication_info_lock);

	FILE *file = fopen(filename, "w");  
    if (!file) {  
        fprintf(stderr, "Failed to open file %s\n", filename);   
		pthread_mutex_unlock(&communication_info_lock);
        return;  
    } 
	pthread_mutex_unlock(&communication_info_lock);

	char* communication_info_array_json_str = parse_communication_info_array_to_json();//这个子函数里面也有加锁解锁操作，记得在调用它之前先把锁给解掉

	pthread_mutex_lock(&communication_info_lock);
	for(int i = 0; i < communication_info_cnt; i++) {
		communication_info_array[i].if_newest_flag = -1;//代表已写入，不是最新的了
	}
    fprintf(file, "%s", communication_info_array_json_str);  
	free(communication_info_array_json_str);
    fclose(file);  
	
	pthread_mutex_unlock(&communication_info_lock);
}






// Function to convert communication_info_array to JSON string  
char* parse_communication_info_array_to_json() {
	pthread_mutex_lock(&communication_info_lock);

    cJSON *json_array = cJSON_CreateArray();  
    for (int i = 0; i < communication_info_cnt; i++) {  
        cJSON *json_obj = cJSON_CreateObject();  
		cJSON_AddStringToObject(json_obj, "linked_node", communication_info_array[i].linked_node);  
        cJSON_AddStringToObject(json_obj, "interface_name", communication_info_array[i].interface_name);  
        char time_buffer[80]; // Assuming enough space for ctime(3) format  
        time_t_to_string(communication_info_array[i].updated_time,time_buffer,sizeof(time_buffer));
        cJSON_AddStringToObject(json_obj, "updated_time", time_buffer);  

		// 将 tx 和 rx 转换为字符串并添加到 JSON 对象中  
		char tx_str[32]; // 假设 tx 的值不会超过 10^9，因此 32 个字符足够  
		char rx_str[32]; // 同上  
		snprintf(tx_str, sizeof(tx_str), "%lu", communication_info_array[i].tx);  
		snprintf(rx_str, sizeof(rx_str), "%lu", communication_info_array[i].rx);  
			  
		cJSON_AddStringToObject(json_obj, "tx", tx_str);  
		cJSON_AddStringToObject(json_obj, "rx", rx_str);  

        //cJSON_AddStringToObject(json_obj, "error_ratio_value", communication_info_array[i].error_ratio_value);  
        cJSON_AddItemToArray(json_array, json_obj);  
    }  
    char *json_string = cJSON_Print(json_array);  
    cJSON_Delete(json_array);  

	pthread_mutex_unlock(&communication_info_lock);
    return json_string;  
}  



// Function to convert communication_info_array to JSON string  
char* parse_communication_info_array_with_certain_listen_interface_to_json(const char *certain_listen_interface) {  
	pthread_mutex_lock(&communication_info_lock);

    cJSON *json_array = cJSON_CreateArray();  
    for (int i = 0; i < communication_info_cnt; i++) {  
		if(strcmp(certain_listen_interface,communication_info_array[i].interface_name)!=0)
			continue;
		
        cJSON *json_obj = cJSON_CreateObject();  
		cJSON_AddStringToObject(json_obj, "linked_node", communication_info_array[i].linked_node);  
        cJSON_AddStringToObject(json_obj, "interface_name", communication_info_array[i].interface_name);  
        char time_buffer[80]; // Assuming enough space for ctime(3) format  
        time_t_to_string(communication_info_array[i].updated_time,time_buffer,sizeof(time_buffer));
        cJSON_AddStringToObject(json_obj, "updated_time", time_buffer);  

		// 将 tx 和 rx 转换为字符串并添加到 JSON 对象中  
		char tx_str[32]; // 假设 tx 的值不会超过 10^9，因此 32 个字符足够  
		char rx_str[32]; // 同上  
		snprintf(tx_str, sizeof(tx_str), "%lu", communication_info_array[i].tx);  
		snprintf(rx_str, sizeof(rx_str), "%lu", communication_info_array[i].rx);  
			  
		cJSON_AddStringToObject(json_obj, "tx", tx_str);  
		cJSON_AddStringToObject(json_obj, "rx", rx_str);  

        //cJSON_AddStringToObject(json_obj, "error_ratio_value", communication_info_array[i].error_ratio_value);  
        cJSON_AddItemToArray(json_array, json_obj);  
    }  
    char *json_string = cJSON_Print(json_array);  
    cJSON_Delete(json_array);  

	pthread_mutex_unlock(&communication_info_lock);
    return json_string;  
}  

// Function to convert communication_info_array to JSON string  
char* parse_newest_communication_infos_to_json() {
	pthread_mutex_lock(&communication_info_lock);

    cJSON *json_array = cJSON_CreateArray();  
    for (int i = 0; i < communication_info_cnt; i++) {  
		if(communication_info_array[i].if_newest_flag<0)
			continue;
		
        cJSON *json_obj = cJSON_CreateObject();  
		cJSON_AddStringToObject(json_obj, "linked_node", communication_info_array[i].linked_node);  
        cJSON_AddStringToObject(json_obj, "interface_name", communication_info_array[i].interface_name);  
        char time_buffer[80]; // Assuming enough space for ctime(3) format  
        time_t_to_string(communication_info_array[i].updated_time,time_buffer,sizeof(time_buffer));
        cJSON_AddStringToObject(json_obj, "updated_time", time_buffer);  

		// 将 tx 和 rx 转换为字符串并添加到 JSON 对象中  
		char tx_str[32]; // 假设 tx 的值不会超过 10^9，因此 32 个字符足够  
		char rx_str[32]; // 同上  
		snprintf(tx_str, sizeof(tx_str), "%lu", communication_info_array[i].tx);  
		snprintf(rx_str, sizeof(rx_str), "%lu", communication_info_array[i].rx);  
			  
		cJSON_AddStringToObject(json_obj, "tx", tx_str);  
		cJSON_AddStringToObject(json_obj, "rx", rx_str);  

        //cJSON_AddStringToObject(json_obj, "error_ratio_value", communication_info_array[i].error_ratio_value);  
        cJSON_AddItemToArray(json_array, json_obj); 
    }  
    char *json_string = cJSON_Print(json_array);  
    cJSON_Delete(json_array);  

	pthread_mutex_unlock(&communication_info_lock);
    return json_string;  
}  

int update_communication_info_array(char* linked_node,char* interface_name,time_t updated_time,unsigned long tx,unsigned long rx) //,char* error_ratio_value
{
	pthread_mutex_lock(&communication_info_lock);
	
	for (size_t i = 0; i < communication_info_cnt; i++) {
        if (strcmp(communication_info_array[i].interface_name, interface_name) == 0) {
			communication_info_array[i].linked_node = linked_node;
			communication_info_array[i].updated_time = updated_time;
			communication_info_array[i].tx = tx;
			communication_info_array[i].rx = rx;
			//communication_info_array[i].error_ratio_value = strdup(error_ratio_value);

			communication_info_array[i].if_newest_flag= 1;//表示是最新的，还没有经过同步
			pthread_mutex_unlock(&communication_info_lock);
            return _SUCCESS;
        }
    }

	pthread_mutex_unlock(&communication_info_lock);
	return _ERROR;

}


void string_to_unsigned_long(const char* str, unsigned long* result) {  
    *result = strtoul(str, NULL, 10); // 使用标准库函数转换  
}  




  
// Function to update the communication_info_array from a JSON string  
int update_communication_info_array_from_json(char* communication_info_array_json_str) {
	pthread_mutex_lock(&communication_info_lock);
	printf("\nUpdate communication info array from json now!\n");

    cJSON *json, *communication_info_item;
	//printf("xsasas\n");
    json = cJSON_Parse(communication_info_array_json_str);
	//printf("26516\n");
    if (!json) { 
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            printf("Error before: %s\n", error_ptr);
        }
		pthread_mutex_unlock(&communication_info_lock);
        return _ERROR;
    }

    if (!cJSON_IsArray(json)) { 
        printf("Error: JSON is not an array\n");
        cJSON_Delete(json);
		pthread_mutex_unlock(&communication_info_lock);
        return _ERROR;
    }

	//printf("xsasas\n");
    int _size = cJSON_GetArraySize(json);
    for (int i = 0; i < _size; i++) {
        communication_info_item = cJSON_GetArrayItem(json, i);
        if (!communication_info_item) continue;

        int found = 0;
        for (int j = 0; j < communication_info_cnt; j++) 
		{
            if (strcmp(communication_info_array[j].interface_name, cJSON_GetObjectItem(communication_info_item, "interface_name")->valuestring) == 0) 
			{

                //free(communication_info_array[j].error_ratio_value);
                free(communication_info_array[j].linked_node);

				communication_info_array[j].linked_node  = strdup(cJSON_GetObjectItem(communication_info_item, "linked_node")->valuestring);
				
				time_t updated_t;
				string_to_time_t(cJSON_GetObjectItem(communication_info_item, "updated_time")->valuestring, &updated_t);
                communication_info_array[j].updated_time = updated_t;

				unsigned long rx_;
				string_to_unsigned_long(cJSON_GetObjectItem(communication_info_item, "rx")->valuestring, &rx_);	
				communication_info_array[j].rx = rx_;  


				unsigned long tx_;
				string_to_unsigned_long(cJSON_GetObjectItem(communication_info_item, "tx")->valuestring, &tx_);	
				communication_info_array[j].tx = tx_; 

				communication_info_array[j].if_newest_flag = 1;//表示是最新的，还没有经过同步
				
                //communication_info_array[j].error_ratio_value = strdup(cJSON_GetObjectItem(communication_info_item, "error_ratio_value")->valuestring);
                found = 1;
				printf("Communication info exist!\n");
				pthread_mutex_unlock(&communication_info_lock);
				print_communication_info(&communication_info_array[j]);
				pthread_mutex_lock(&communication_info_lock);
                break;
            }
        }
        if (!found) {
            communication_info_cnt++;
			//free_communication_info_array()；
            communication_info_array = realloc(communication_info_array, communication_info_cnt * sizeof(struct communication_info));
            if (!communication_info_array) {
                printf("Error: Memory allocation failed\n");
                continue; // 或者处理错误
            }
			communication_info_array[communication_info_cnt - 1].linked_node = strdup(cJSON_GetObjectItem(communication_info_item, "linked_node")->valuestring);
            communication_info_array[communication_info_cnt - 1].interface_name = strdup(cJSON_GetObjectItem(communication_info_item, "interface_name")->valuestring);
            time_t updated_t;
			string_to_time_t(cJSON_GetObjectItem(communication_info_item, "updated_time")->valuestring, &updated_t);
            communication_info_array[communication_info_cnt - 1].updated_time = updated_t;

			unsigned long tx_;
			unsigned long rx_;
			// 转换并存储tx和rx  
			string_to_unsigned_long(cJSON_GetObjectItem(communication_info_item, "tx")->valuestring, &tx_); 
			communication_info_array[communication_info_cnt - 1].tx = tx_;  
			string_to_unsigned_long(cJSON_GetObjectItem(communication_info_item, "rx")->valuestring, &rx_); 
			communication_info_array[communication_info_cnt - 1].rx = rx_;  

			communication_info_array[communication_info_cnt - 1].if_newest_flag = 1;//表示是最新的，还没有经过同步

			printf("Communication info added!\n");
			pthread_mutex_unlock(&communication_info_lock);
			print_communication_info(&communication_info_array[communication_info_cnt-1]);
			pthread_mutex_lock(&communication_info_lock);
			//communication_info_array[communication_info_cnt - 1].error_ratio_value = strdup(cJSON_GetObjectItem(communication_info_item, "error_ratio_value")->valuestring);
        }
    }
	//printf("xsasxas\n");
    cJSON_Delete(json);
    // 清理通信信息数组中未使用的条目
	pthread_mutex_unlock(&communication_info_lock);
	return  _SUCCESS;
}


int get_if_newest_flag(int i)
{
	return communication_info_array[i].if_newest_flag;
}

int set_if_newest_flag(int i,int flag)
{
	communication_info_array[i].if_newest_flag = flag;
}

int read_communication_info_array_from_json(const char *filename) {
	pthread_mutex_lock(&communication_info_lock);
	
	char buffer[BUFFER_SIZE];
	FILE *file = fopen(filename, "r");  
    if (file == NULL) {  
        printf("open failed\n");  
        return _ERROR;  
    }  
    int i = 0;  
    while (i < BUFFER_SIZE - 1 && fscanf(file, "%c", &buffer[i]) == 1) {
        i++;  
    }
	if (i < BUFFER_SIZE) { 
		buffer[i] = '\0';  
    }  
	fclose(file);

	free_communication_info_array();
	malloc_communication_info_array(0);
	int ret = update_communication_info_array_from_json(buffer);
	pthread_mutex_unlock(&communication_info_lock);
	return ret;
}


void write_interface_info_array_to_json(const char *filename, struct interface_info *array, size_t size) {  
    cJSON *interfaces = cJSON_CreateArray();  
    if (!interfaces) {  
        fprintf(stderr, "Memory allocation failed\n");  
        return;  
    }  
  
    for (size_t i = 0; i < size; i++) {  
        cJSON *interface = cJSON_CreateObject();  
        if (!interface) {  
            fprintf(stderr, "Memory allocation failed\n");  
            cJSON_Delete(interfaces);  
            return;  
        }  
  
        cJSON_AddStringToObject(interface, "located_node", array[i].located_node);  
        cJSON_AddStringToObject(interface, "interface_name", array[i].interface_name);  
        cJSON_AddStringToObject(interface, "interface_type", array[i].interface_type);  
        cJSON_AddStringToObject(interface, "linked_node", array[i].linked_node);  
        cJSON_AddStringToObject(interface, "linked_interface_name", array[i].linked_interface_name);  
        cJSON_AddStringToObject(interface, "linked_interface_type", array[i].linked_interface_type);  
		cJSON_AddStringToObject(interface, "center_interface_name", array[i].center_interface_name);  
		cJSON_AddStringToObject(interface, "status_chooser", array[i].status_chooser);  
		cJSON_AddStringToObject(interface, "base_send_func", array[i].base_send_func);  
		cJSON_AddStringToObject(interface, "base_receive_func", array[i].base_receive_func); 
		cJSON_AddStringToObject(interface, "msg_generator_of_sender", array[i].msg_generator_of_sender);
		cJSON_AddStringToObject(interface, "initializer_name", array[i].initializer_name);
		cJSON_AddStringToObject(interface, "closer_name", array[i].closer_name);


		cJSON *eth_info = cJSON_CreateObject();  
		if (array[i].eth_info.mac_addr != NULL) {  
		    cJSON_AddStringToObject(eth_info, "ip_addr", array[i].eth_info.ip_addr);  
			cJSON_AddStringToObject(eth_info, "net_mask", array[i].eth_info.net_mask);  
			cJSON_AddStringToObject(eth_info, "mac_addr", array[i].eth_info.mac_addr); 
			cJSON_AddStringToObject(eth_info, "ip_name", array[i].eth_info.ip_name); 
		}  
		cJSON_AddItemToObject(interface, "eth_info", eth_info);  


		cJSON *linked_eth_info = cJSON_CreateObject();  
		if (array[i].linked_eth_info.mac_addr != NULL) {  
		    cJSON_AddStringToObject(linked_eth_info, "ip_addr", array[i].linked_eth_info.ip_addr);  
			cJSON_AddStringToObject(linked_eth_info, "net_mask", array[i].linked_eth_info.net_mask);
		    cJSON_AddStringToObject(linked_eth_info, "mac_addr", array[i].linked_eth_info.mac_addr);  
			cJSON_AddStringToObject(linked_eth_info, "ip_name", array[i].linked_eth_info.ip_name);  
		}  
		cJSON_AddItemToObject(interface, "linked_eth_info", linked_eth_info); 


		cJSON *can_info = cJSON_CreateObject();  
		if (array[i].can_info.can_id != -1) {   // 假设 -1 是一个无效值，用于检查  
		    cJSON_AddNumberToObject(can_info, "can_id", array[i].can_info.can_id);
			cJSON_AddNumberToObject(can_info, "baud_rate", array[i].can_info.baud_rate);
		}
		cJSON_AddItemToObject(interface, "can_info", can_info);

		cJSON *linked_can_info = cJSON_CreateObject();  
		if (array[i].linked_can_info.can_id != -1) {   // 假设 -1 是一个无效值，用于检查  
		    cJSON_AddNumberToObject(linked_can_info, "can_id", array[i].linked_can_info.can_id); 
			cJSON_AddNumberToObject(linked_can_info, "baud_rate", array[i].linked_can_info.baud_rate);
		}
		cJSON_AddItemToObject(interface, "linked_can_info", linked_can_info);


	
		cJSON *rs485_info = cJSON_CreateObject();  
		if (array[i].rs485_info.baud_rate != -1) { // 假设 -1 是一个无效值，用于检查  
			cJSON_AddNumberToObject(rs485_info, "rs485_gpio_number", array[i].rs485_info.rs485_gpio_number); 
		    cJSON_AddNumberToObject(rs485_info, "databits", array[i].rs485_info.databits); 
			cJSON_AddNumberToObject(rs485_info, "stopbits", array[i].rs485_info.stopbits);
			cJSON_AddNumberToObject(rs485_info, "paritybits", array[i].rs485_info.paritybits);
			cJSON_AddNumberToObject(rs485_info, "baud_rate", array[i].rs485_info.baud_rate);
			cJSON_AddStringToObject(rs485_info, "rs485_dev_path", array[i].rs485_info.rs485_dev_path);
		}  
		cJSON_AddItemToObject(interface, "rs485_info", rs485_info); 



		cJSON *linked_rs485_info = cJSON_CreateObject();  
		if (array[i].linked_rs485_info.baud_rate != -1) { // 假设 -1 是一个无效值，用于检查  
			cJSON_AddNumberToObject(linked_rs485_info, "rs485_gpio_number", array[i].linked_rs485_info.rs485_gpio_number);
		    cJSON_AddNumberToObject(linked_rs485_info, "databits", array[i].linked_rs485_info.databits); 
			cJSON_AddNumberToObject(linked_rs485_info, "stopbits", array[i].linked_rs485_info.stopbits);
			cJSON_AddNumberToObject(linked_rs485_info, "paritybits", array[i].linked_rs485_info.paritybits);
			cJSON_AddNumberToObject(linked_rs485_info, "baud_rate", array[i].linked_rs485_info.baud_rate); 
			cJSON_AddStringToObject(linked_rs485_info, "rs485_dev_path", array[i].linked_rs485_info.rs485_dev_path);
		}  
		cJSON_AddItemToObject(interface, "linked_rs485_info", linked_rs485_info); 


  		cJSON_AddStringToObject(interface, "mode", array[i].mode);  
        cJSON_AddStringToObject(interface, "status", array[i].status); 
		cJSON_AddStringToObject(interface, "duplex", array[i].duplex); 
		
  
        cJSON_AddItemToArray(interfaces, interface);  
    }  
  
    char *rendered = cJSON_Print(interfaces);  
    if (rendered == NULL) {  
        fprintf(stderr, "Failed to render JSON\n");  
        cJSON_Delete(interfaces);  
        return;  
    }  
  
    FILE *file = fopen(filename, "w");  
    if (!file) {  
        fprintf(stderr, "Failed to open file %s\n", filename);  
        cJSON_Delete(interfaces);  
        free(rendered);  
        return;  
    }  
  
    fprintf(file, "%s", rendered);  
    fclose(file);  
  
    free(rendered);  
    cJSON_Delete(interfaces);  
}  


int update_interface_cnt(const char *filename)
{
	cJSON *json;
	char buffer[BUFFER_SIZE];
	FILE *file = fopen(filename, "r");  
    if (file == NULL) {  
        printf("open failed\n");  
        return 1;  
    }  
    int i = 0;  
    while (i < BUFFER_SIZE - 1 && fscanf(file, "%c", &buffer[i]) == 1) {
        i++;  
    }
	if (i < BUFFER_SIZE) { 
		buffer[i] = '\0';  
    }  
	fclose(file);

    json = cJSON_Parse(buffer);  
    if (!json) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        return -1;
    }

    if (!cJSON_IsArray(json)) {
        fprintf(stderr, "Error: JSON is not an array\n");
        cJSON_Delete(json);
        return -1;
    }
    return cJSON_GetArraySize(json);
}



void    read_interface_info_array_from_json(const char *filename, struct interface_info *array,size_t size) {
	cJSON *json, *interfaces, *interface, *tmp; 
	char buffer[BUFFER_SIZE];
	FILE *file = fopen(filename, "r");  
    if (file == NULL) {  
        printf("open failed\n");  
        return _ERROR;  
    }  
    int i = 0;  
    while (i < BUFFER_SIZE - 1 && fscanf(file, "%c", &buffer[i]) == 1) {
        i++;  
    }
	if (i < BUFFER_SIZE) { 
		buffer[i] = '\0';  
    }  
	fclose(file);

	//printf("%d      %d      %s\n",i,BUFFER_SIZE,buffer);

    json = cJSON_Parse(buffer);  
    if (!json) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            fprintf(stderr, "Error before: %s\n", error_ptr);
			fprintf(stderr, "Buffer content: %s\n", buffer);
        }
        return;
    }

    if (!cJSON_IsArray(json)) {
        fprintf(stderr, "Error: JSON is not an array\n");
        cJSON_Delete(json);
        return;
    }
    interfaces = json;
    int _size = cJSON_GetArraySize(interfaces);
	if(_size>size)
	{
		_size=size;
	}
	//printf("xasxasx\n");
    for (int i = 0; i < _size; i++) {
        interface = cJSON_GetArrayItem(interfaces, i);
        if (!interface) continue;

		array[i].initialized_flag = -1; //表示还未初始化

        // Allocate and fill located_node
        array[i].located_node = strdup(cJSON_GetObjectItem(interface, "located_node")->valuestring);
        // Similarly fill other fields...
        array[i].interface_name = strdup(cJSON_GetObjectItem(interface, "interface_name")->valuestring);
        array[i].interface_type = strdup(cJSON_GetObjectItem(interface, "interface_type")->valuestring);
        array[i].linked_node = strdup(cJSON_GetObjectItem(interface, "linked_node")->valuestring);
        array[i].linked_interface_name = strdup(cJSON_GetObjectItem(interface, "linked_interface_name")->valuestring);
        array[i].linked_interface_type = strdup(cJSON_GetObjectItem(interface, "linked_interface_type")->valuestring);
		array[i].center_interface_name = strdup(cJSON_GetObjectItem(interface, "center_interface_name")->valuestring);
		array[i].status_chooser = strdup(cJSON_GetObjectItem(interface, "status_chooser")->valuestring);
		array[i].base_send_func = strdup(cJSON_GetObjectItem(interface, "base_send_func")->valuestring);
		array[i].base_receive_func = strdup(cJSON_GetObjectItem(interface, "base_receive_func")->valuestring);
		array[i].msg_generator_of_sender = strdup(cJSON_GetObjectItem(interface, "msg_generator_of_sender")->valuestring);
		array[i].initializer_name = strdup(cJSON_GetObjectItem(interface, "initializer_name")->valuestring);
		array[i].closer_name = strdup(cJSON_GetObjectItem(interface, "closer_name")->valuestring);


        array[i].status = strdup(cJSON_GetObjectItem(interface, "status")->valuestring);
		array[i].mode = strdup(cJSON_GetObjectItem(interface, "mode")->valuestring);
		array[i].duplex = strdup(cJSON_GetObjectItem(interface, "duplex")->valuestring);


        // Handle nested structures
		// Fill eth_info  
		tmp = cJSON_GetObjectItem(interface, "eth_info");  
		if (tmp && cJSON_IsObject(tmp)) {  
			cJSON *ip_name_item = cJSON_GetObjectItem(tmp, "ip_name");  
			if (ip_name_item && cJSON_IsString(ip_name_item)) {  
				array[i].eth_info.ip_name = strdup(ip_name_item->valuestring);  
			} else {  
				array[i].eth_info.ip_name = NULL;	
			}  
			
			cJSON *ip_addr_item = cJSON_GetObjectItem(tmp, "ip_addr");  
			if (ip_addr_item && cJSON_IsString(ip_addr_item)) {  
				array[i].eth_info.ip_addr = strdup(ip_addr_item->valuestring);  
			} else {  
				array[i].eth_info.ip_addr = NULL;	
			}  

			cJSON *net_mask_item = cJSON_GetObjectItem(tmp, "net_mask");  
			if (net_mask_item && cJSON_IsString(net_mask_item)) {  
				array[i].eth_info.net_mask = strdup(net_mask_item->valuestring);  
			} else {  
				array[i].eth_info.net_mask = NULL;	
			}  

			cJSON *mac_addr_item = cJSON_GetObjectItem(tmp, "mac_addr");  
			if (mac_addr_item && cJSON_IsString(mac_addr_item)) {  
				array[i].eth_info.mac_addr = strdup(mac_addr_item->valuestring);  
			} else {  
				array[i].eth_info.mac_addr = NULL;	
			}  
		} else {  
			array[i].eth_info.ip_name = NULL;
			array[i].eth_info.ip_addr = NULL;
			array[i].eth_info.net_mask = NULL;
			array[i].eth_info.mac_addr = NULL;	
		}  

		// Fill linked_eth_info  
		tmp = cJSON_GetObjectItem(interface, "linked_eth_info");  
		if (tmp && cJSON_IsObject(tmp)) { 
			cJSON *ip_name_item = cJSON_GetObjectItem(tmp, "ip_name");  
			if (ip_name_item && cJSON_IsString(ip_name_item)) {  
				array[i].linked_eth_info.ip_name = strdup(ip_name_item->valuestring);  
			} else {  
				array[i].linked_eth_info.ip_name = NULL;	
			} 

			
			cJSON *ip_addr_item = cJSON_GetObjectItem(tmp, "ip_addr");  
			if (ip_addr_item && cJSON_IsString(ip_addr_item)) {  
				array[i].linked_eth_info.ip_addr = strdup(ip_addr_item->valuestring);  
			} else {  
				array[i].linked_eth_info.ip_addr = NULL;	
			}  

			cJSON *net_mask_item = cJSON_GetObjectItem(tmp, "net_mask");  
			if (net_mask_item && cJSON_IsString(net_mask_item)) {  
				array[i].linked_eth_info.net_mask = strdup(net_mask_item->valuestring);  
			} else {  
				array[i].linked_eth_info.net_mask = NULL;	
			}  

			cJSON *mac_addr_item = cJSON_GetObjectItem(tmp, "mac_addr");  
			if (mac_addr_item && cJSON_IsString(mac_addr_item)) {  
				array[i].linked_eth_info.mac_addr = strdup(mac_addr_item->valuestring);  
			} else {  
				array[i].linked_eth_info.mac_addr = NULL;	
			}  
		} else {  
			array[i].linked_eth_info.ip_name = NULL;
			array[i].linked_eth_info.ip_addr = NULL;
			array[i].linked_eth_info.net_mask = NULL;
			array[i].linked_eth_info.mac_addr = NULL;		
		} 



		// Fill can_id	
		tmp = cJSON_GetObjectItem(interface, "can_info");  
		if (tmp && cJSON_IsObject(tmp)) {  
			cJSON *can_id_item = cJSON_GetObjectItem(tmp, "can_id");  
			if (can_id_item && cJSON_IsNumber(can_id_item)) {  
				array[i].can_info.can_id = can_id_item->valuedouble;  
			} else {  
				array[i].can_info.can_id = -1;  
			}  
			
			cJSON *baud_rate_item = cJSON_GetObjectItem(tmp, "baud_rate");  
			if (baud_rate_item && cJSON_IsNumber(baud_rate_item)) {  
				array[i].can_info.baud_rate = baud_rate_item->valuedouble;  
			} else {  
				array[i].can_info.baud_rate = -1;  
			} 
		} else {  
			array[i].can_info.can_id = -1;  
			array[i].can_info.baud_rate = -1;  
		}  


		// Fill linked_can_id	
		tmp = cJSON_GetObjectItem(interface, "linked_can_info");  
		if (tmp && cJSON_IsObject(tmp)) {  
			cJSON *can_id_item = cJSON_GetObjectItem(tmp, "can_id");  
			if (can_id_item && cJSON_IsNumber(can_id_item)) {  
				array[i].linked_can_info.can_id = can_id_item->valuedouble;  
			} else {  
				array[i].linked_can_info.can_id = -1;  
			} 

			cJSON *baud_rate_item = cJSON_GetObjectItem(tmp, "baud_rate");  
			if (baud_rate_item && cJSON_IsNumber(baud_rate_item)) {  
				array[i].linked_can_info.baud_rate = baud_rate_item->valuedouble;  
			} else {  
				array[i].linked_can_info.baud_rate = -1;  
			} 
		} else {  
			array[i].linked_can_info.can_id = -1;  
			array[i].linked_can_info.baud_rate = -1;  
		}



		// Fill baud_rate  
		tmp = cJSON_GetObjectItem(interface, "rs485_info");  
		if (tmp && cJSON_IsObject(tmp)) {  

			cJSON *rs485_dev_path_item = cJSON_GetObjectItem(tmp, "rs485_dev_path");	
			if (rs485_dev_path_item && cJSON_IsString(rs485_dev_path_item)) {  
				array[i].rs485_info.rs485_dev_path = strdup(rs485_dev_path_item->valuestring);  
			} else {  
				array[i].rs485_info.rs485_dev_path= NULL;  
			}  

			
			cJSON *rs485_gpio_number_item = cJSON_GetObjectItem(tmp, "rs485_gpio_number");	
			if (rs485_gpio_number_item && cJSON_IsNumber(rs485_gpio_number_item)) {  
				array[i].rs485_info.rs485_gpio_number = rs485_gpio_number_item->valuedouble;  
			} else {  
				array[i].rs485_info.rs485_gpio_number = -1;  
			}  

			
			cJSON *baud_rate_item = cJSON_GetObjectItem(tmp, "baud_rate");	
			if (baud_rate_item && cJSON_IsNumber(baud_rate_item)) {  
				array[i].rs485_info.baud_rate = baud_rate_item->valuedouble;  
			} else {  
				array[i].rs485_info.baud_rate = -1;  
			}  

			cJSON *databits_item = cJSON_GetObjectItem(tmp, "databits");
			if (databits_item && cJSON_IsNumber(databits_item)) {  
				array[i].rs485_info.databits = databits_item->valuedouble;  
			} else {  
				array[i].rs485_info.databits = -1;  
			}  

			cJSON *stopbits_item = cJSON_GetObjectItem(tmp, "stopbits");
			if (stopbits_item && cJSON_IsNumber(stopbits_item)) {  
				array[i].rs485_info.stopbits = stopbits_item->valuedouble;  
			} else {  
				array[i].rs485_info.stopbits = -1;  
			}  

			cJSON *paritybits_item = cJSON_GetObjectItem(tmp, "paritybits");
			if (paritybits_item && cJSON_IsNumber(paritybits_item)) {  
				array[i].rs485_info.paritybits = paritybits_item->valuedouble;  
			} else {  
				array[i].rs485_info.paritybits = 0;  
			}  

	
		} else {  
			array[i].rs485_info.rs485_dev_path = NULL; 
			array[i].rs485_info.rs485_gpio_number = -1; 
			array[i].rs485_info.baud_rate = -1;  
			array[i].rs485_info.databits = -1;  
			array[i].rs485_info.stopbits = -1;  
			array[i].rs485_info.paritybits = 0;  
		}


		// Fill linked_baud_rate  
		tmp = cJSON_GetObjectItem(interface, "linked_rs485_info");  
		if (tmp && cJSON_IsObject(tmp)) { 

			cJSON *rs485_dev_path_item = cJSON_GetObjectItem(tmp, "rs485_dev_path");	
			if (rs485_dev_path_item && cJSON_IsString(rs485_dev_path_item)) {  
				array[i].linked_rs485_info.rs485_dev_path = strdup(rs485_dev_path_item->valuestring);  
			} else {  
				array[i].linked_rs485_info.rs485_dev_path= NULL;  
			}  

			
			cJSON *rs485_gpio_number_item = cJSON_GetObjectItem(tmp, "rs485_gpio_number");	
			if (rs485_gpio_number_item && cJSON_IsNumber(rs485_gpio_number_item)) {  
				array[i].linked_rs485_info.rs485_gpio_number = rs485_gpio_number_item->valuedouble;  
			} else {  
				array[i].linked_rs485_info.rs485_gpio_number = -1;  
			}  
			
			cJSON *baud_rate_item = cJSON_GetObjectItem(tmp, "baud_rate");	
			if (baud_rate_item && cJSON_IsNumber(baud_rate_item)) {  
				array[i].linked_rs485_info.baud_rate = baud_rate_item->valuedouble;  
			} else {  
				array[i].linked_rs485_info.baud_rate = -1;  
			}  

			cJSON *databits_item = cJSON_GetObjectItem(tmp, "databits");
			if (databits_item && cJSON_IsNumber(databits_item)) {  
				array[i].linked_rs485_info.databits = databits_item->valuedouble;  
			} else {  
				array[i].linked_rs485_info.databits = -1;  
			}  

			cJSON *stopbits_item = cJSON_GetObjectItem(tmp, "stopbits");
			if (stopbits_item && cJSON_IsNumber(stopbits_item)) {  
				array[i].linked_rs485_info.stopbits = stopbits_item->valuedouble;  
			} else {  
				array[i].linked_rs485_info.stopbits = -1;  
			}  

			cJSON *paritybits_item = cJSON_GetObjectItem(tmp, "paritybits");
			if (paritybits_item && cJSON_IsNumber(paritybits_item)) {  
				array[i].linked_rs485_info.paritybits = paritybits_item->valuedouble;  
			} else {  
				array[i].linked_rs485_info.paritybits = 0;  
			}  

	
		} else {  
			array[i].linked_rs485_info.rs485_dev_path = NULL; 
			array[i].linked_rs485_info.rs485_gpio_number = -1; 
			array[i].linked_rs485_info.baud_rate = -1;  
			array[i].linked_rs485_info.databits = -1;  
			array[i].linked_rs485_info.stopbits = -1;  
			array[i].linked_rs485_info.paritybits = 0;  
		}

    }

    cJSON_Delete(json);
}


void malloc_interface_info_array(int size)
{
	interface_cnt = size;	
	interface_info_array = (struct interface_info *)malloc(interface_cnt * sizeof(struct interface_info));	
}

void malloc_communication_info_array(int size)
{
	communication_info_cnt = size;	
	communication_info_array = (struct communication_info *)malloc(communication_info_cnt * sizeof(struct communication_info));	
}


void free_interface_info_array()
{
	for (int i = 0; i < interface_cnt; i++) {  
		free(interface_info_array[i].located_node);  
		free(interface_info_array[i].interface_name);  
		free(interface_info_array[i].interface_type);  
		free(interface_info_array[i].linked_node);	
		free(interface_info_array[i].linked_interface_name);  
		free(interface_info_array[i].linked_interface_type);
		free(interface_info_array[i].center_interface_name);
		free(interface_info_array[i].status_chooser);
		free(interface_info_array[i].base_send_func);
		free(interface_info_array[i].base_receive_func);
		free(interface_info_array[i].msg_generator_of_sender);
		free(interface_info_array[i].initializer_name);
		free(interface_info_array[i].closer_name);


		free(interface_info_array[i].eth_info.ip_name);
		free(interface_info_array[i].eth_info.ip_addr);
		free(interface_info_array[i].eth_info.net_mask);
		free(interface_info_array[i].eth_info.mac_addr);  
		free(interface_info_array[i].linked_eth_info.ip_name);
		free(interface_info_array[i].linked_eth_info.ip_addr);
		free(interface_info_array[i].linked_eth_info.net_mask);
		free(interface_info_array[i].linked_eth_info.mac_addr);
		free(interface_info_array[i].rs485_info.rs485_dev_path);
		free(interface_info_array[i].linked_rs485_info.rs485_dev_path);
		free(interface_info_array[i].status); 
		free(interface_info_array[i].mode); 
		free(interface_info_array[i].duplex); 
	}  
	free(interface_info_array);
}

void free_communication_info_array()
{
	for (int i = 0; i < communication_info_cnt; i++) {
		free(communication_info_array[i].linked_node);
		free(communication_info_array[i].interface_name);
		free(communication_info_array[i].updated_time);
		//free(communication_info_array[i].error_ratio_value);
	}
	free(communication_info_array);
}


time_t get_test_begin_time()
{
	return test_begin_time;
}


void start_and_load_info(const char *filename)
{

	printf("loading interface information......\n");

	int size = update_interface_cnt(filename);
	int ret = string_to_time_t(TEST_BEIGIN_TIME, &test_begin_time);
	
	if (ret!=_SUCCESS) {
		test_begin_time = 0;
		printf("cannot parse the test begin time!\n");
	}

	
	malloc_interface_info_array(size);
	read_interface_info_array_from_json(filename,interface_info_array,interface_cnt);
	
	initialize_communication_info_lock();
	
	int count=0;
	for(int i=0;i<size;i++)
	{
		if(strcmp(interface_info_array[i].mode,"listen")==0)
			count++;
	}

	communication_info_cnt = count;
	malloc_communication_info_array(count);


	int c_i=0;
	
	for (int i = 0; i < size; i++) {
		if(strcmp(interface_info_array[i].mode,"listen")==0)
		{
			communication_info_array[c_i].linked_node = strdup(interface_info_array[i].linked_node);
			communication_info_array[c_i].interface_name = strdup(interface_info_array[i].interface_name);
			time_t now = time(NULL); 
			communication_info_array[c_i].updated_time = time(NULL);
			communication_info_array[c_i].tx = PAKCAGES_NUM_ONE_TIME;
			communication_info_array[c_i].rx = 0;
			//communication_info_array[i].error_ratio_value = "0";
			c_i++;
		}
	}

	printf("loaded\n");
	
}

void dump_info_and_close(const char *filename)
{
	write_interface_info_array_to_json(filename,interface_info_array,interface_cnt);
	free_interface_info_array();
	free_communication_info_array();

	destroy_communication_info_lock();
}



int get_interface_cnt()
{
	return interface_cnt;
}





char *get_located_node(int i)
{
	return interface_info_array[i].located_node;
}

char *get_linked_node(int i)
{
	return interface_info_array[i].linked_node;
}


char* get_base_send_func_by_index(int i)
{
	return interface_info_array[i].base_send_func;
}

char* get_base_receive_func_by_index(int i)
{
	return interface_info_array[i].base_receive_func;
}

char* get_initializer_name_by_index(int i)
{
	return interface_info_array[i].initializer_name;
}

char* get_closer_name_by_index(int i)
{
	return interface_info_array[i].closer_name;
}


char* get_interface_name_by_index(int i)
{
	return interface_info_array[i].interface_name;
}

char* get_linked_interface_name_by_index(int i)
{
	return interface_info_array[i].linked_interface_name;
}


int get_interface_index(const char* interface_name)
{
    for (size_t i = 0; i < interface_cnt; i++) {
        if (strcmp(interface_info_array[i].interface_name, interface_name) == 0) {
            return i;
        }
    }
    return -1;

}

char*  get_msg_generator_of_sender_by_index(int i)
{
	return interface_info_array[i].msg_generator_of_sender;
}



int  get_rs485_baud_rate_by_index(int i)
{
	return interface_info_array[i].rs485_info.baud_rate;
}


int  get_can_baud_rate_by_index(int i)
{
	return interface_info_array[i].can_info.baud_rate;
}


int  get_databits_by_index(int i)
{
	return interface_info_array[i].rs485_info.databits;
}


unsigned char  get_paritybits_by_index(int i)
{
	return interface_info_array[i].rs485_info.paritybits;
}


int  get_stopbits_by_index(int i)
{
	return interface_info_array[i].rs485_info.stopbits;
}

int  get_temporary_fd(int i)
{
	return interface_info_array[i].rs485_info.temporary_fd;
}

void  set_temporary_fd(int i,int fd)
{
	interface_info_array[i].rs485_info.temporary_fd = fd;
}


time_t  get_last_work_time(int i)
{
	return interface_info_array[i].last_work_time;
}

void  set_last_work_time(int i,time_t now)
{
	interface_info_array[i].last_work_time = now;
}





char* get_rs485_dev_path_by_index(int i)
{
	return interface_info_array[i].rs485_info.rs485_dev_path;
}


char* get_ip_name_by_index(int i)
{
	return interface_info_array[i].eth_info.ip_name;
}


char* get_interface_status(const char* interface_name)
{
    for (size_t i = 0; i < interface_cnt; i++) {
        if (strcmp(interface_info_array[i].interface_name, interface_name) == 0) {
            return interface_info_array[i].status;
        }
    }
    return NULL;

}

char* get_interface_status_by_index(int i)
{
    return interface_info_array[i].status;

}


// Function to check if the status is valid  
bool isValidStatus(const char* status) {  
    static const char* validStatuses[] = {"sending", "receiving", "sending_and_receiving", "closed"};  
    size_t numStatuses = sizeof(validStatuses) / sizeof(validStatuses[0]);  
    for (size_t i = 0; i < numStatuses; i++) {  
        if (strcmp(status, validStatuses[i]) == 0) {  
            return true;  
        }  
    }  
    return false;  
}  


char* get_ip_addr_by_index(int i)
{
	return interface_info_array[i].eth_info.ip_addr;
}


char* get_net_mask_by_index(int i)
{
	return interface_info_array[i].eth_info.net_mask;
}



int get_rs485_gpio_number_by_index(int i)
{
	return interface_info_array[i].rs485_info.rs485_gpio_number;
}




int get_channel_id_by_index(int i)
{
	return interface_info_array[i].can_info.can_id;
}





  
int set_interface_status(const char* interface_name, const char* status)  
{  
    // Check if the status is valid  
    if (!isValidStatus(status)) {  
        return _ERROR; // Return error if status is invalid  
    }  
  
    // Iterate through the interface array to find the matching interface  
    for (size_t i = 0; i < interface_cnt; i++) {  
        if (strcmp(interface_info_array[i].interface_name, interface_name) == 0) {  
			free(interface_info_array[i].status);
            // Use strdup to allocate and copy the new status  
            interface_info_array[i].status = strdup(status);  
            if (interface_info_array[i].status == NULL) {  
                // Handle memory allocation failure  
                return _ERROR;  
            }  
            return _SUCCESS; // Return success if the interface was found and updated  
        }  
    }  
    return _ERROR; // Return error if the interface was not found  
}



char* get_interface_mode(const char* interface_name)
{
    for (size_t i = 0; i < interface_cnt; i++) {
        if (strcmp(interface_info_array[i].interface_name, interface_name) == 0) {
            return interface_info_array[i].mode;
        }
    }
    return NULL;

}


char* get_mac_addr(const char *interface_name) {
    for (size_t i = 0; i < interface_cnt; i++) {
        if (strcmp(interface_info_array[i].interface_name, interface_name) == 0) {
            if (strcmp(interface_info_array[i].interface_type, "eth") == 0) {
                return interface_info_array[i].eth_info.mac_addr;
            }
        }
    }
    return NULL;
}

char* get_linked_mac_addr(const char *interface_name) {
    for (size_t i = 0; i < interface_cnt; i++) {
        if (strcmp(interface_info_array[i].interface_name, interface_name) == 0) {
            if (strcmp(interface_info_array[i].interface_type, "eth") == 0) {
                return interface_info_array[i].linked_eth_info.mac_addr;
            }
        }
    }
    return NULL;
}

char* get_interface_type_by_index(int i)
{
    return interface_info_array[i].interface_type;
}

char* get_interface_duplex_by_index(int i)
{
    return interface_info_array[i].duplex;
}

char* get_status_chooser_by_index(int i)
{
    return interface_info_array[i].status_chooser;
}



char* get_interface_mode_by_index(int i)
{
    return interface_info_array[i].mode;
}

char* get_interface_type(const char *interface_name)
{
    for (size_t i = 0; i < interface_cnt; i++) {
        if (strcmp(interface_info_array[i].interface_name, interface_name) == 0) {
            return interface_info_array[i].interface_type;
        }
    }
    return NULL;

}


char* get_center_interface_name(int i)
{
	return interface_info_array[i].center_interface_name;
}

void print_communication_info(const struct communication_info*info)
{
	pthread_mutex_lock(&communication_info_lock);
    printf("Linked Node: %s\n", info->linked_node);  
    printf("Listened Interface Name: %s\n", info->interface_name);  
	char time_buffer[80]; // Assuming enough space for ctime(3) format  
    time_t_to_string(info->updated_time,time_buffer,sizeof(time_buffer));
    printf("Updated Time: %s\n", time_buffer);  
    printf("Tx Number: %d\n", info->tx);  
	printf("Rx Number: %d\n", info->rx);  
	pthread_mutex_unlock(&communication_info_lock);
}



void print_communication_info_array(const struct communication_info *array, int size) {  
    if (array == NULL || size <= 0) {  
        printf("communication info array is NULL or size is invalid\n");  
        return;  
    }  
  	printf("\n");
    for (int i = 0; i < size; i++) {    
        print_communication_info(&array[i]);  
        printf("\n"); // 添加空行以分隔不同的接口信息  
    }  
}

void printAllCommucationInfo()
{
	print_communication_info_array(communication_info_array,communication_info_cnt);
}

void printNewestCommucationInfo()
{
    if (communication_info_array == NULL || communication_info_cnt <= 0) {  
        printf("communication info array is NULL or size is invalid\n");  
        return;  
    }  
  	printf("\n");
    for (int i = 0; i < communication_info_cnt; i++) {  
		if(communication_info_array[i].if_newest_flag>0)
        	print_communication_info(&communication_info_array[i]);
		printf("\n"); // 添加空行以分隔不同的接口信息  
    }  
}


// Function to print interface information  
// Function to print interface information  
void print_interface_info(const struct interface_info *info) {  
    if (info == NULL) {  
        printf("Interface info is NULL\n");  
        return;  
    }  
  
    printf("Located Node: %s\n", info->located_node);  
    printf("Interface Name: %s\n", info->interface_name);  
    printf("Interface Type: %s\n", info->interface_type);  
    printf("Linked Node: %s\n", info->linked_node);  
    printf("Linked Interface Name: %s\n", info->linked_interface_name);  
    printf("Linked Interface Type: %s\n", info->linked_interface_type);
	printf("Status Chooser: %s\n", info->status_chooser);
	printf("Base Send Func: %s\n", info->base_send_func);
	printf("Base Receive Func: %s\n", info->base_receive_func);
	printf("Msg Generator Of Sender: %s\n", info->msg_generator_of_sender); 
    printf("Mode: %s\n", info->mode);
    printf("Status: %s\n", info->status);

    printf("Ethernet Info:\n");  
    printf("\tIP Address: %s\n", info->eth_info.ip_addr);  
    printf("\tNet Mask: %s\n", info->eth_info.net_mask);  
    printf("\tMAC Address: %s\n", info->eth_info.mac_addr);  
  
    printf("Linked Ethernet Info:\n");  
    printf("\tIP Address: %s\n", info->linked_eth_info.ip_addr);  
    printf("\tNet Mask: %s\n", info->linked_eth_info.net_mask);  
    printf("\tMAC Address: %s\n", info->linked_eth_info.mac_addr);  
  
    printf("CAN Info:\n");  
    printf("\tCAN ID: %d\n", info->can_info.can_id);  
	printf("\tCAN baud_rate: %d\n", info->linked_can_info.baud_rate);
  
    printf("Linked CAN Info:\n");  
    printf("\tCAN ID: %d\n", info->linked_can_info.can_id); 
	printf("\tCAN baud_rate: %d\n", info->linked_can_info.baud_rate);
  
    printf("RS485 Info:\n");  
    printf("\tGPIO Number: %d\n", info->rs485_info.rs485_gpio_number);  
    printf("\tData Bits: %d\n", info->rs485_info.databits);  
    printf("\tStop Bits: %d\n", info->rs485_info.stopbits);  
    printf("\tParity Bits: %u\n", info->rs485_info.paritybits);  
    printf("\tBaud Rate: %d\n", info->rs485_info.baud_rate);  
    printf("\tTemporary FD: %d\n", info->rs485_info.temporary_fd);  
  
    printf("Linked RS485 Info:\n");  
    printf("\tGPIO Number: %d\n", info->linked_rs485_info.rs485_gpio_number);  
    printf("\tData Bits: %d\n", info->linked_rs485_info.databits);  
    printf("\tStop Bits: %d\n", info->linked_rs485_info.stopbits);  
    printf("\tParity Bits: %u\n", info->linked_rs485_info.paritybits);  
    printf("\tBaud Rate: %d\n", info->linked_rs485_info.baud_rate);  
    printf("\tTemporary FD: %d\n", info->linked_rs485_info.temporary_fd);  
  



}



void print_interface_info_array(const struct interface_info *array, int size) {  
    if (array == NULL || size <= 0) {  
        printf("Interface info array is NULL or size is invalid\n");  
        return;  
    }  
  
    for (int i = 0; i < size; i++) {  
        printf("Interface ID: %d\n", i + 1);  
        print_interface_info(&array[i]);  
        printf("\n"); // 添加空行以分隔不同的接口信息  
    }  
}



void printAllInfo()
{
	print_interface_info_array(interface_info_array,interface_cnt);
}

#if 0
int main() {  
    interface_cnt = 3;  
    interface_info_array = (struct interface_info *)malloc(interface_cnt * sizeof(struct interface_info));  
  
    if (interface_info_array == NULL) {  
        fprintf(stderr, "Memory allocation failed\n");  
        return 1;  
    }  
  
    // 初始化第一个接口  
    interface_info_array[0].located_node = strdup("Node1");  
    interface_info_array[0].interface_name = strdup("Eth0");  
    interface_info_array[0].interface_type = strdup("Ethernet");  
    interface_info_array[0].linked_node = strdup("Node2");  
    interface_info_array[0].linked_interface_name = strdup("Eth1");  
    interface_info_array[0].linked_interface_type = strdup("Ethernet");  
    interface_info_array[0].eth_info.mac_addr = strdup("00:1A:2B:3C:4D:5E");  
    interface_info_array[0].can_info.can_id = 123;  
    interface_info_array[0].rs485_info.baud_rate = 9600;  
    interface_info_array[0].status = strdup("Active");  
  
    // 初始化第二个接口  
    interface_info_array[1].located_node = strdup("Node2");  
    interface_info_array[1].interface_name = strdup("Can0");	
    interface_info_array[1].interface_type = strdup("CAN");  
    interface_info_array[1].linked_node = strdup("Node4");  // 假设链接到Node4  
    interface_info_array[1].linked_interface_name = strdup("Can1");  // 假设链接到Can1  
    interface_info_array[1].linked_interface_type = strdup("CAN");  // 链接接口类型也是CAN  
    interface_info_array[1].eth_info.mac_addr = NULL;  // 对于CAN接口，mac_addr可能不适用，因此设置为NULL  
    interface_info_array[1].can_info.can_id = 456;  // 假设CAN ID为456  
    interface_info_array[1].rs485_info.baud_rate = 0;  // 对于CAN接口，baud_rate可能不适用，因此设置为0  
    interface_info_array[1].status = strdup("Active");  
	
  // 初始化第三个接口  
    interface_info_array[2].located_node = strdup("Node3");  
    interface_info_array[2].interface_name = strdup("Rs485");  
    interface_info_array[2].interface_type = strdup("RS485");  
    interface_info_array[2].linked_node = strdup("Node5");  // 假设链接到Node5  
    interface_info_array[2].linked_interface_name = strdup("Rs485_2");  // 假设链接到另一个RS485接口	
    interface_info_array[2].linked_interface_type = strdup("RS485");	// 链接接口类型也是RS485  
    interface_info_array[2].eth_info.mac_addr = NULL;  // 对于RS485接口，mac_addr不适用  
    interface_info_array[2].can_info.can_id = 0;	// 对于RS485接口，can_id不适用	
    interface_info_array[2].rs485_info.baud_rate = 115200;  // 设置波特率为115200  
    interface_info_array[2].status = strdup("Inactive");	// 假设该接口当前处于非活动状态


	write_interface_info_array_to_json("test.json",interface_info_array,3);
	// 在这里可以使用 interface_info_array 进行测试或进一步处理  
	read_interface_info_array_from_json("test.json",interface_info_array,&interface_cnt);
	write_interface_info_array_to_json("test.json",interface_info_array,3);

  
    // 释放内存（不要忘记释放所有通过 strdup 分配的内存）  
    for (int i = 0; i < interface_cnt; i++) {  
        free(interface_info_array[i].located_node);  
        free(interface_info_array[i].interface_name);  
        free(interface_info_array[i].interface_type);  
        free(interface_info_array[i].linked_node);  
        free(interface_info_array[i].linked_interface_name);  
        free(interface_info_array[i].linked_interface_type);  
        free(interface_info_array[i].eth_info.mac_addr);  
		free(interface_info_array[i].linked_eth_info.mac_addr);  
        free(interface_info_array[i].status);  
    }  
    free(interface_info_array);  
  
    return 0;  
}

#endif


