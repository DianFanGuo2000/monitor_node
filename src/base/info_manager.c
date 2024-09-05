

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


void string_to_unsigned_long(const char* str, unsigned long* result) {  
    *result = strtoul(str, NULL, 10); // 使用标准库函数转换  
}  


void string_to_int(const char* str, int* result) 
{
	// 使用标准库函数 strtol 将字符串转换为 int 类型      
	*result = strtol(str, NULL, 10);  
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


//查找node子节点中标签为contentName的值
char* getChildNodeContentWithSpecifiedName(xmlNode *node, const char *contentName) {  
    xmlNode *curNode = NULL;  
    for (curNode = node->children; curNode != NULL; curNode = curNode->next) {  
        if ((!xmlStrcmp(curNode->name, (const xmlChar *)contentName))) {  
            xmlChar *content = xmlNodeGetContent(curNode);  
            char *result = (char *)malloc(strlen((const char *)content) + 1);  
            strcpy(result, (const char *)content);  
            xmlFree(content);  
            return result;  
        }  
    }  
    return NULL;  
}  
  




/*
xml 文件分为
(1) 节点接口定义文件 

<nodes>  
    <node>  
        <node_name>fpu-testee</node_name>
        <interface> 
            <id>0</id>  
            <label>GE1</label>  
            <interface_type>eth</interface_type>  
            <status_chooser>work_at_each_round</status_chooser>  
            <base_send_func>send_packet</base_send_func>  
            <base_receive_func>receive_packet</base_receive_func>  
            <msg_generator_of_sender>generate_eth_test_msg</msg_generator_of_sender>  
            <initializer_name>eth_initializer_normal</initializer_name>  
            <closer_name>eth_closer_normal</closer_name>  
            <eth_info>  
                <ip_name>enp3s0</ip_name>  
                <ip_addr>192.168.1.10</ip_addr>  
                <net_mask>255.255.255.0</net_mask>  
                <mac_addr>00:80:0f:74:10:22</mac_addr>  
            </eth_info>  
            <status>sending_and_receiving</status>  
            <duplex>full-duplex</duplex>  
        </interface>

    </node>

    <node>  
        <node_name>fpu-cotestee</node_name>
        <interface> 
            <id>100</id>  
            <label>GE7</label>  
            <interface_type>eth</interface_type>  
            <status_chooser>work_at_each_round</status_chooser>  
            <base_send_func>send_packet</base_send_func>  
            <base_receive_func>receive_packet</base_receive_func>  
            <msg_generator_of_sender>generate_eth_test_msg</msg_generator_of_sender>  
            <initializer_name>eth_initializer_normal</initializer_name>  
            <closer_name>eth_closer_normal</closer_name>  
            <eth_info>  
                <ip_name>enp5s0</ip_name>  
                <ip_addr>192.168.1.12</ip_addr>  
                <net_mask>255.255.255.0</net_mask>  
                <mac_addr>00:80:0f:74:10:24</mac_addr>  
            </eth_info>  
            <status>sending_and_receiving</status>  
            <duplex>full-duplex</duplex>  
        </interface>
    </node>  
</nodes>


(2) 边link定义文件 

<links>  
    <link> 
        <send_if_id>0</send_if_id>
        <listened_if_id>100</listened_if_id>
    </link>
    <link> 
        <send_if_id>100</send_if_id>
        <listened_if_id>0</listened_if_id>
    </link>
</links>

以上两个文件对应以下json总配置文件:
[
{
	"mode":       "test",
	"located_node": "fpu-testee",
	"interface_name":       "fpu-testee-GE7-100-test",
	"interface_type":       "eth",
	"center_interface_name":        "fpu-cotestee-GE7-100-listen",
	"status_chooser":        "work_at_each_round",
	"base_send_func":        "send_packet",
	"base_receive_func":        "receive_packet",
	"msg_generator_of_sender":        "generate_eth_test_msg",
	"initializer_name":        "eth_initializer_normal",
	"closer_name":        "eth_closer_normal",
	"eth_info":     {
		"ip_name":     "enp5s0",
		"ip_addr":     "192.168.1.12",
		"net_mask":     "255.255.255.0",
		"mac_addr":     "00:80:0f:74:10:24"
	},
	"status":       "sending_and_receiving",
	"duplex":       "full-duplex",
	
	"linked_interface_name":        "fpu-cotestee-GE1-0-listen"
},{
	"mode":       "listen",
	"located_node": "fpu-testee",
	"interface_name":       "fpu-testee-GE7-100-listen",
	"interface_type":       "eth",
	"center_interface_name":        "fpu-cotestee-GE7-100-listen",
	"status_chooser":        "work_at_each_round",
	"base_send_func":        "send_packet",
	"base_receive_func":        "receive_packet",
	"msg_generator_of_sender":        "generate_eth_test_msg",
	"initializer_name":        "eth_initializer_normal",
	"closer_name":        "eth_closer_normal",
	"eth_info":     {
		"ip_name":     "enp5s0",
		"ip_addr":     "192.168.1.12",
		"net_mask":     "255.255.255.0",
		"mac_addr":     "00:80:0f:74:10:24"
	},
	"status":       "sending_and_receiving",
	"duplex":       "full-duplex",
	
	"linked_interface_name":        "fpu-cotestee-GE1-0-test"
},{
	"mode":       "listen",
	"located_node": "fpu-cotestee",
	"interface_name":       "fpu-cotestee-GE1-0-listen",
	"interface_type":       "eth",
	"center_interface_name":        "fpu-cotestee-GE7-100-listen",
	"status_chooser":        "work_at_each_round",
	"base_send_func":        "send_packet",
	"base_receive_func":        "receive_packet",
	"msg_generator_of_sender":        "generate_eth_test_msg",
	"initializer_name":        "eth_initializer_normal",
	"closer_name":        "eth_closer_normal",
	"eth_info":     {
		"ip_name":     "enp3s0",
		"ip_addr":     "192.168.1.110",
		"net_mask":     "255.255.255.0",
		"mac_addr":     "00:80:0f:74:30:02"
	},
	"status":       "sending_and_receiving",
	"duplex":       "full-duplex",
	
	"linked_interface_name":        "fpu-testee-GE7-100-test"
},{
	"mode":       "test",
	"located_node": "fpu-cotestee",
	"interface_name":       "fpu-cotestee-GE1-0-test",
	"interface_type":       "eth",
	"center_interface_name":        "fpu-cotestee-GE7-100-listen",
	"status_chooser":        "work_at_each_round",
	"base_send_func":        "send_packet",
	"base_receive_func":        "receive_packet",
	"msg_generator_of_sender":        "generate_eth_test_msg",
	"initializer_name":        "eth_initializer_normal",
	"closer_name":        "eth_closer_normal",
	"eth_info":     {
		"ip_name":     "enp3s0",
		"ip_addr":     "192.168.1.110",
		"net_mask":     "255.255.255.0",
		"mac_addr":     "00:80:0f:74:30:02"
	},
	"status":       "sending_and_receiving",
	"duplex":       "full-duplex",
	
	"linked_interface_name":        "fpu-testee-GE7-100-listen"
}]

*/
char *get_target_if_value_from_node_tree(xmlNode *rootNode_node_if, char *if_id, char *if_property_name) {  
    for (xmlNode *node_t = rootNode_node_if->children; node_t; node_t = node_t->next) {  
        xmlNode *interfaceNode = NULL; // 目标接口节点  
        for (xmlNode *node = node_t->children; node; node = node->next) {  
            char *id = getChildNodeContentWithSpecifiedName(node, "id");  
            if (id && strcmp(id, if_id) == 0) {  
                interfaceNode = node;  
                break;  
            }  
        }  
          
        if (interfaceNode) {  
            // 检查常见属性  
            if (strcmp(if_property_name, "label") == 0 ||  
                strcmp(if_property_name, "interface_type") == 0 ||  
                strcmp(if_property_name, "status_chooser") == 0 ||  
                strcmp(if_property_name, "base_send_func") == 0 ||  
                strcmp(if_property_name, "base_receive_func") == 0 ||  
                strcmp(if_property_name, "msg_generator_of_sender") == 0 ||  
                strcmp(if_property_name, "initializer_name") == 0 ||  
                strcmp(if_property_name, "closer_name") == 0 ||  
                strcmp(if_property_name, "status") == 0 ||  
                strcmp(if_property_name, "duplex") == 0) {  
                  
                char *propertyValue = getChildNodeContentWithSpecifiedName(interfaceNode, if_property_name);  
                if (propertyValue) {  
                    return propertyValue;  
                }  
            }  
              
            // 检查特定属性  
            if (strcmp(if_property_name, "ip_name") == 0 ||  
                strcmp(if_property_name, "ip_addr") == 0 ||  
                strcmp(if_property_name, "net_mask") == 0 ||  
                strcmp(if_property_name, "mac_addr") == 0 ||
                strcmp(if_property_name, "port_id") == 0) {  
                  
                for (xmlNode *node = interfaceNode->children; node; node = node->next) {  
                    char *if_property_value = getChildNodeContentWithSpecifiedName(node, if_property_name);  
                    if (if_property_value) {  
                        return if_property_value;  
                    }  
                }  
            }

			if (strcmp(if_property_name, "rs485_dev_path") == 0 ||  
                strcmp(if_property_name, "rs485_gpio_number") == 0 ||  
                strcmp(if_property_name, "baud_rate") == 0 ||  
                strcmp(if_property_name, "databits") == 0 ||
                strcmp(if_property_name, "stopbits") == 0 ||
                strcmp(if_property_name, "paritybits") == 0) {  
                  
                for (xmlNode *node = interfaceNode->children; node; node = node->next) {  
                    char *if_property_value = getChildNodeContentWithSpecifiedName(node, if_property_name);  
                    if (if_property_value) {  
                        return if_property_value;  
                    }  
                }  
            } 

			if (strcmp(if_property_name, "can_id") == 0 ||  
                strcmp(if_property_name, "baud_rate") == 0) {  
                  
                for (xmlNode *node = interfaceNode->children; node; node = node->next) {  
                    char *if_property_value = getChildNodeContentWithSpecifiedName(node, if_property_name);  
                    if (if_property_value) {  
                        return if_property_value;  
                    }  
                }  
            } 
        }  
    }  
    return NULL;  
}


char *get_node_name_contain_target_if_from_node_tree(xmlNode *rootNode_node_if, char *if_id) {  
    for (xmlNode *node_t = rootNode_node_if->children; node_t; node_t = node_t->next) { 
		char *node_name = getChildNodeContentWithSpecifiedName(node_t, "node_name");
        for (xmlNode *node = node_t->children; node; node = node->next) {
            char *id = getChildNodeContentWithSpecifiedName(node, "id");  
            if (id && strcmp(id, if_id) == 0) {  
                return node_name;  
            }  
        }  
    }
    return NULL;  
}

char* concatenateStringsWithDelimiter(const char* str1, const char* str2, const char* str3, const char* str4) {  
    // 计算需要的总长度（包括分隔符和结尾的'\0'）  
    int totalLength = strlen(str1) + strlen(str2) + strlen(str3) + strlen(str4) + 4; 
  
    // 分配足够的内存来存储结果字符串  
    char* result = (char*)malloc(totalLength * sizeof(char));  
    if (result == NULL) {  
        printf("内存分配失败");  
        exit(1); // 终止程序  
    }  
  
    // 使用snprintf安全地拼接字符串  
    snprintf(result, totalLength, "%s-%s-%s-%s", str1, str2, str3, str4);  
  
    return result;  
}  





char* xmlToJson(const char *xmlData_node_if, const char *xmlData_link) {  

	
    // 解析XML  
    xmlDocPtr doc_node_if = xmlParseDoc((const xmlChar *)xmlData_node_if);  
    xmlDocPtr doc_link = xmlParseDoc((const xmlChar *)xmlData_link);  
  
    xmlNode *rootNode_node_if = xmlDocGetRootElement(doc_node_if);  
    xmlNode *rootNode_link = xmlDocGetRootElement(doc_link);  

	
  
    // 创建一个JSON数组  
    cJSON *jsonArray = cJSON_CreateArray();  



    // 遍历link节点  
    for (xmlNode *linkNode = rootNode_link->children; linkNode; linkNode = linkNode->next) 
	{    //遍历 rootNode_link 的子节点，就是一个个<link></link>区间
        if (linkNode->type == XML_ELEMENT_NODE) 
		{    
            char *ifIdStr = getChildNodeContentWithSpecifiedName(linkNode, "send_if_id");  
            char *listenIfIdStr = getChildNodeContentWithSpecifiedName(linkNode, "listened_if_id");
			char *centerIfIdStr = getChildNodeContentWithSpecifiedName(linkNode, "center_if_id");

	        cJSON *jsonObject_test = cJSON_CreateObject();  

			char *center_node = get_node_name_contain_target_if_from_node_tree(rootNode_node_if,centerIfIdStr);
			char *center_label = get_target_if_value_from_node_tree(rootNode_node_if,centerIfIdStr, "label");
			char *center_interface_name = concatenateStringsWithDelimiter(center_node,center_label,centerIfIdStr,"listen");


			char *located_node = get_node_name_contain_target_if_from_node_tree(rootNode_node_if,ifIdStr);



			char *label = get_target_if_value_from_node_tree(rootNode_node_if,ifIdStr, "label");
			char* interface_name = concatenateStringsWithDelimiter(located_node,label,ifIdStr,"test");
			char* interface_type = get_target_if_value_from_node_tree(rootNode_node_if,ifIdStr, "interface_type");
			char* status_chooser = get_target_if_value_from_node_tree(rootNode_node_if,ifIdStr, "status_chooser");
			char* base_send_func = get_target_if_value_from_node_tree(rootNode_node_if,ifIdStr, "base_send_func");
			char* base_receive_func = get_target_if_value_from_node_tree(rootNode_node_if,ifIdStr, "base_receive_func");
			char* msg_generator_of_sender = get_target_if_value_from_node_tree(rootNode_node_if,ifIdStr, "msg_generator_of_sender");
			char* initializer_name = get_target_if_value_from_node_tree(rootNode_node_if,ifIdStr, "initializer_name");
			char* closer_name = get_target_if_value_from_node_tree(rootNode_node_if,ifIdStr, "closer_name");
			char* status = get_target_if_value_from_node_tree(rootNode_node_if,ifIdStr, "status");
			char* duplex = get_target_if_value_from_node_tree(rootNode_node_if,ifIdStr, "duplex");


			char *listened_node = get_node_name_contain_target_if_from_node_tree(rootNode_node_if,listenIfIdStr);


			char *listened_label = get_target_if_value_from_node_tree(rootNode_node_if,listenIfIdStr, "label");
			char* listened_interface_name = concatenateStringsWithDelimiter(listened_node,listened_label,listenIfIdStr,"listen");
			
			char* listened_interface_type = get_target_if_value_from_node_tree(rootNode_node_if,listenIfIdStr, "interface_type");
			char* listened_status_chooser = get_target_if_value_from_node_tree(rootNode_node_if,listenIfIdStr, "status_chooser");
			char* listened_base_send_func = get_target_if_value_from_node_tree(rootNode_node_if,listenIfIdStr, "base_send_func");
			char* listened_base_receive_func = get_target_if_value_from_node_tree(rootNode_node_if,listenIfIdStr, "base_receive_func");
			char* listened_msg_generator_of_sender = get_target_if_value_from_node_tree(rootNode_node_if,listenIfIdStr, "msg_generator_of_sender");
			char* listened_initializer_name = get_target_if_value_from_node_tree(rootNode_node_if,listenIfIdStr, "initializer_name");
			char* listened_closer_name = get_target_if_value_from_node_tree(rootNode_node_if,listenIfIdStr, "closer_name");
			char* listened_status = get_target_if_value_from_node_tree(rootNode_node_if,listenIfIdStr, "status");
			char* listened_duplex = get_target_if_value_from_node_tree(rootNode_node_if,listenIfIdStr, "duplex");
	  

			char* ip_name = get_target_if_value_from_node_tree(rootNode_node_if,ifIdStr, "ip_name");
			char* ip_addr = get_target_if_value_from_node_tree(rootNode_node_if,ifIdStr, "ip_addr");
			char* net_mask = get_target_if_value_from_node_tree(rootNode_node_if,ifIdStr, "net_mask");
			char* mac_addr = get_target_if_value_from_node_tree(rootNode_node_if,ifIdStr, "mac_addr");
			char* l_port_id = get_target_if_value_from_node_tree(rootNode_node_if,ifIdStr, "port_id");


			char* rs485_dev_path = get_target_if_value_from_node_tree(rootNode_node_if,ifIdStr, "rs485_dev_path");
			char* rs485_gpio_number = get_target_if_value_from_node_tree(rootNode_node_if,ifIdStr, "rs485_gpio_number");
			char* rs485_baud_rate = get_target_if_value_from_node_tree(rootNode_node_if,ifIdStr, "baud_rate");
			char* databits = get_target_if_value_from_node_tree(rootNode_node_if,ifIdStr, "databits");
			char* stopbits = get_target_if_value_from_node_tree(rootNode_node_if,ifIdStr, "stopbits");
			char* paritybits = get_target_if_value_from_node_tree(rootNode_node_if,ifIdStr, "paritybits");
			


			char* can_id = get_target_if_value_from_node_tree(rootNode_node_if,ifIdStr, "can_id");
			char* can_baud_rate = get_target_if_value_from_node_tree(rootNode_node_if,ifIdStr, "baud_rate");
			

			char* listened_ip_name = get_target_if_value_from_node_tree(rootNode_node_if,listenIfIdStr, "ip_name");
			char* listened_ip_addr = get_target_if_value_from_node_tree(rootNode_node_if,listenIfIdStr, "ip_addr");
			char* listened_net_mask = get_target_if_value_from_node_tree(rootNode_node_if,listenIfIdStr, "net_mask");
			char* listened_mac_addr = get_target_if_value_from_node_tree(rootNode_node_if,listenIfIdStr, "mac_addr");
			char* l_l_port_id = get_target_if_value_from_node_tree(rootNode_node_if,listenIfIdStr, "port_id");


			char* listened_rs485_dev_path = get_target_if_value_from_node_tree(rootNode_node_if,listenIfIdStr, "rs485_dev_path");
			char* listened_rs485_gpio_number = get_target_if_value_from_node_tree(rootNode_node_if,listenIfIdStr, "rs485_gpio_number");
			char* listened_rs485_baud_rate = get_target_if_value_from_node_tree(rootNode_node_if,listenIfIdStr, "baud_rate");
			char* listened_databits = get_target_if_value_from_node_tree(rootNode_node_if,listenIfIdStr, "databits");
			char* listened_stopbits = get_target_if_value_from_node_tree(rootNode_node_if,listenIfIdStr, "stopbits");
			char* listened_paritybits = get_target_if_value_from_node_tree(rootNode_node_if,listenIfIdStr, "paritybits");


			char* listened_can_id = get_target_if_value_from_node_tree(rootNode_node_if,listenIfIdStr, "can_id");
			char* listened_can_baud_rate = get_target_if_value_from_node_tree(rootNode_node_if,listenIfIdStr, "baud_rate");
			

			//printf("%s %s\n",l_port_id,l_l_port_id);
			
	        // 添加属性  
	        cJSON_AddStringToObject(jsonObject_test, "mode", "test");  
	        cJSON_AddStringToObject(jsonObject_test, "located_node", located_node); 
	        cJSON_AddStringToObject(jsonObject_test, "interface_name", interface_name);  
	        cJSON_AddStringToObject(jsonObject_test, "interface_type", interface_type); 
			cJSON_AddStringToObject(jsonObject_test, "center_interface_name", center_interface_name);
			cJSON_AddStringToObject(jsonObject_test, "status_chooser", status_chooser);
			cJSON_AddStringToObject(jsonObject_test, "base_send_func", base_send_func);
			cJSON_AddStringToObject(jsonObject_test, "base_receive_func", base_receive_func);
			cJSON_AddStringToObject(jsonObject_test, "msg_generator_of_sender", msg_generator_of_sender);
			cJSON_AddStringToObject(jsonObject_test, "initializer_name", initializer_name);
			cJSON_AddStringToObject(jsonObject_test, "closer_name", closer_name);
			cJSON_AddStringToObject(jsonObject_test, "status", status);
			cJSON_AddStringToObject(jsonObject_test, "duplex", duplex);



	  		if(strcmp(interface_type,"eth")==0)
	  		{
		        // 处理eth_info  
		        cJSON *ethInfo = cJSON_CreateObject();  
		        cJSON_AddStringToObject(ethInfo, "ip_name", ip_name);  
		        cJSON_AddStringToObject(ethInfo, "ip_addr", ip_addr);  
		        cJSON_AddStringToObject(ethInfo, "net_mask", net_mask);  
		        cJSON_AddStringToObject(ethInfo, "mac_addr", mac_addr);  
				int t_;
				string_to_int(l_port_id,&t_);
				cJSON_AddNumberToObject(ethInfo, "port_id", t_); 
		        cJSON_AddItemToObject(jsonObject_test, "eth_info", ethInfo);  
	  		}




	  		if(strcmp(interface_type,"rs485")==0)
	  		{
		        cJSON *rs485Info = cJSON_CreateObject();  
		        cJSON_AddStringToObject(rs485Info, "rs485_dev_path", rs485_dev_path);  

				
				
				int t_;
				string_to_int(rs485_gpio_number,&t_);
				//printf("%s %d\n",rs485_gpio_number,t_);
		        cJSON_AddNumberToObject(rs485Info, "rs485_gpio_number", t_);  

				string_to_int(rs485_baud_rate,&t_);
				//printf("%s %d\n",rs485_baud_rate,t_);
		        cJSON_AddNumberToObject(rs485Info, "baud_rate", t_);  

				string_to_int(databits,&t_);
				//printf("%s %d\n",databits,t_);
		        cJSON_AddNumberToObject(rs485Info, "databits", t_);  

				string_to_int(stopbits,&t_);
				//printf("%s %d\n",stopbits,t_);
				cJSON_AddNumberToObject(rs485Info, "stopbits", t_); 

				string_to_int(paritybits,&t_);
				//printf("%s %d\n",paritybits,t_);
		        cJSON_AddNumberToObject(rs485Info, "paritybits", t_);  

				
		        cJSON_AddItemToObject(jsonObject_test, "rs485_info", rs485Info);  

				
	  		}


	  		if(strcmp(interface_type,"can")==0)
	  		{
		        // 处理eth_info  
		        cJSON *canInfo = cJSON_CreateObject();  

				int t_;
				string_to_int(can_id,&t_);
				//printf("%s %d\n",can_id,t_);
		        cJSON_AddNumberToObject(canInfo, "can_id", t_);  

				string_to_int(can_baud_rate,&t_);
				//printf("%s %d\n",can_baud_rate,t_);
		        cJSON_AddNumberToObject(canInfo, "baud_rate", t_); 
				
		        cJSON_AddItemToObject(jsonObject_test, "can_info", canInfo);  
	  		}


			 
			cJSON_AddStringToObject(jsonObject_test, "linked_interface_name", listened_interface_name);

	        cJSON_AddItemToArray(jsonArray, jsonObject_test);  

			
			//----------------------------------------------------------------

			cJSON *jsonObject_listen = cJSON_CreateObject();  
			
			// 添加属性  
			cJSON_AddStringToObject(jsonObject_listen, "mode", "listen");  
			cJSON_AddStringToObject(jsonObject_listen, "located_node", listened_node); 
			cJSON_AddStringToObject(jsonObject_listen, "interface_name", listened_interface_name);  
			cJSON_AddStringToObject(jsonObject_listen, "interface_type", listened_interface_type);	
			cJSON_AddStringToObject(jsonObject_listen, "center_interface_name", center_interface_name);
			cJSON_AddStringToObject(jsonObject_listen, "status_chooser", listened_status_chooser);
			cJSON_AddStringToObject(jsonObject_listen, "base_send_func", listened_base_send_func);
			cJSON_AddStringToObject(jsonObject_listen, "base_receive_func", listened_base_receive_func);
			cJSON_AddStringToObject(jsonObject_listen, "msg_generator_of_sender", listened_msg_generator_of_sender);
			cJSON_AddStringToObject(jsonObject_listen, "initializer_name", listened_initializer_name);
			cJSON_AddStringToObject(jsonObject_listen, "closer_name", listened_closer_name);
			cJSON_AddStringToObject(jsonObject_listen, "status", listened_status);
			cJSON_AddStringToObject(jsonObject_listen, "duplex", listened_duplex);
			
			
			if(strcmp(listened_interface_type,"eth")==0)
			{
				// 处理eth_info  
				cJSON *ethInfo = cJSON_CreateObject();	
		        cJSON_AddStringToObject(ethInfo, "ip_name", listened_ip_name);  
		        cJSON_AddStringToObject(ethInfo, "ip_addr", listened_ip_addr);  
		        cJSON_AddStringToObject(ethInfo, "net_mask", listened_net_mask);  
		        cJSON_AddStringToObject(ethInfo, "mac_addr", listened_mac_addr);   
				int t_;
				string_to_int(l_l_port_id,&t_);
				cJSON_AddNumberToObject(ethInfo, "port_id", t_); 
				cJSON_AddItemToObject(jsonObject_listen, "eth_info", ethInfo);  
			}


			
	  		if(strcmp(listened_interface_type,"rs485")==0)
	  		{
		        // 处理eth_info  
		        cJSON *rs485Info = cJSON_CreateObject();  
		        cJSON_AddStringToObject(rs485Info, "rs485_dev_path", listened_rs485_dev_path);  


				int t_;
				string_to_int(listened_rs485_gpio_number,&t_);
		        cJSON_AddNumberToObject(rs485Info, "rs485_gpio_number", t_);  

				string_to_int(listened_rs485_baud_rate,&t_);
		        cJSON_AddNumberToObject(rs485Info, "baud_rate", t_);  

				string_to_int(listened_databits,&t_);
		        cJSON_AddNumberToObject(rs485Info, "databits", t_);  

				string_to_int(listened_stopbits,&t_);
				cJSON_AddNumberToObject(rs485Info, "stopbits", t_); 

				string_to_int(listened_paritybits,&t_);
		        cJSON_AddNumberToObject(rs485Info, "paritybits", t_); 

				
		        cJSON_AddItemToObject(jsonObject_listen, "rs485_info", rs485Info);  
	  		}


	  		if(strcmp(listened_interface_type,"can")==0)
	  		{
		        cJSON *canInfo = cJSON_CreateObject();  
				
				int t_;
				string_to_int(listened_can_id,&t_);
		        cJSON_AddNumberToObject(canInfo, "can_id", t_);  

				string_to_int(listened_can_baud_rate,&t_);
		        cJSON_AddNumberToObject(canInfo, "baud_rate", t_); 
				
		        cJSON_AddItemToObject(jsonObject_listen, "can_info", canInfo);  
	  		}
			 
			cJSON_AddStringToObject(jsonObject_listen, "linked_interface_name", interface_name);
			
			cJSON_AddItemToArray(jsonArray, jsonObject_listen);  

			free(ifIdStr);
			free(listenIfIdStr);
			free(centerIfIdStr);
					
			free(center_interface_name);
			free(center_node);
			free(center_label);


			free(interface_name);
			free(located_node);
			free(label);
			
			free(listened_interface_name);
			free(listened_node);
			free(listened_label);
			
			
			free(interface_type);
			free(status_chooser);
			free(base_send_func);
			free(base_receive_func);
			free(msg_generator_of_sender);
			free(initializer_name);
			free(closer_name);
			free(status);
			free(duplex);
			free(ip_name);
			free(ip_addr);
			free(net_mask);
			free(mac_addr);
			free(l_port_id);
			free(rs485_dev_path);
			free(rs485_gpio_number);
			free(rs485_baud_rate);
			free(databits);
			free(stopbits);
			free(paritybits);
			free(can_id);
			free(can_baud_rate);

			
			free(listened_interface_type);
			free(listened_status_chooser);
			free(listened_base_send_func);
			free(listened_base_receive_func);
			free(listened_msg_generator_of_sender);
			free(listened_initializer_name);
			free(listened_closer_name);
			free(listened_status);
			free(listened_duplex);
			free(listened_ip_name);
			free(listened_ip_addr);
			free(listened_net_mask);
			free(listened_mac_addr);
			free(l_l_port_id);
			free(listened_rs485_dev_path);
			free(listened_rs485_gpio_number);
			free(listened_rs485_baud_rate);
			free(listened_databits);
			free(listened_stopbits);
			free(listened_paritybits);
			free(listened_can_id);
			free(listened_can_baud_rate);

        }  
    }  

    // 将cJSON对象转换为字符串  
    char *jsonString = cJSON_Print(jsonArray);  
    cJSON_Delete(jsonArray);  
  
    // 清理XML文档  
    xmlFreeDoc(doc_node_if);  
    xmlFreeDoc(doc_link);  
  
    return jsonString;  
}  


void convert_xml_config_to_overall_json_config(char *xml_config_path_node_if, char *xml_config_path_node_link, char *overall_topology_json_config_path) {

	printf("***********************************start to convert xml config(path from: %s, %s) to overall json config(path to:%s)\n",xml_config_path_node_if,xml_config_path_node_link,overall_topology_json_config_path);

	char buffer1[BUFFER_SIZE];
	FILE *file = fopen(xml_config_path_node_if, "r");  
    if (file == NULL) {  
        printf("open failed\n");  
        return _ERROR;  
    }  
    int i = 0;  
    while (i < BUFFER_SIZE - 1 && fscanf(file, "%c", &buffer1[i]) == 1) {
        i++;  
    }
	if (i < BUFFER_SIZE) { 
		buffer1[i] = '\0';  
    }  
	fclose(file);


	char buffer2[BUFFER_SIZE];
	file = fopen(xml_config_path_node_link, "r");  
    if (file == NULL) {  
        printf("open failed\n");  
        return _ERROR;  
    }  
    i = 0;  
    while (i < BUFFER_SIZE - 1 && fscanf(file, "%c", &buffer2[i]) == 1) {
        i++;  
    }
	if (i < BUFFER_SIZE) { 
		buffer2[i] = '\0';  
    }  
	fclose(file);

	
	char *overall_topology_json_str = xmlToJson(buffer1,buffer2);

	file = fopen(overall_topology_json_config_path, "w");  
    if (!file) {  
        fprintf(stderr, "Failed to open file %s\n", overall_topology_json_config_path);   
        return;  
    } 
	fprintf(file, "%s", overall_topology_json_str);
	free(overall_topology_json_str); // 释放由cJSON_Print分配的字符串  
    fclose(file);  

	printf("***********************************converted successfully\n");
	
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


		char current_round_str[32];
		snprintf(current_round_str, sizeof(current_round_str), "%d", communication_info_array[i].current_round);  
		cJSON_AddStringToObject(json_obj, "current_round", current_round_str);  
		

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

		char current_round_str[32];
		snprintf(current_round_str, sizeof(current_round_str), "%d", communication_info_array[i].current_round);  
		cJSON_AddStringToObject(json_obj, "current_round", current_round_str);  


        //cJSON_AddStringToObject(json_obj, "error_ratio_value", communication_info_array[i].error_ratio_value);  
        cJSON_AddItemToArray(json_array, json_obj);  
    }  
    char *json_string = cJSON_Print(json_array);  
    cJSON_Delete(json_array);  

	pthread_mutex_unlock(&communication_info_lock);
    return json_string;  
}  

// Function to convert communication_info_array to JSON string  
int memoried=0;
char* parse_newest_communication_infos_to_json(int max_info_num_one_time, int whether_used_for_sync) {
	pthread_mutex_lock(&communication_info_lock);


	int cnt = 0;
	
    cJSON *json_array = cJSON_CreateArray(); 
    for (int i = memoried; i < communication_info_cnt; i++) {  
		memoried = i;
		if(communication_info_array[i].if_newest_flag<0)
			continue;

		if(whether_used_for_sync>0)
			communication_info_array[i].if_newest_flag = -1;

		cnt++; // 计数，开始转化第一个info为字符串
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

		char current_round_str[32];
		snprintf(current_round_str, sizeof(current_round_str), "%d", communication_info_array[i].current_round);  
		cJSON_AddStringToObject(json_obj, "current_round", current_round_str);  


        //cJSON_AddStringToObject(json_obj, "error_ratio_value", communication_info_array[i].error_ratio_value);  
        cJSON_AddItemToArray(json_array, json_obj); 

		if(max_info_num_one_time > 0 && cnt == max_info_num_one_time)
			break;

    }  
	if(memoried==communication_info_cnt-1)
	{
		memoried=0;
	}
    char *json_string = cJSON_Print(json_array);  
    cJSON_Delete(json_array);  

	pthread_mutex_unlock(&communication_info_lock);
    return json_string;  
}  

int update_communication_info_array(char* linked_node,char* interface_name,time_t updated_time,unsigned long tx,unsigned long rx,int current_round) //,char* error_ratio_value
{
	pthread_mutex_lock(&communication_info_lock);
	
	for (size_t i = 0; i < communication_info_cnt; i++) {
        if (strcmp(communication_info_array[i].interface_name, interface_name) == 0) {
			free(communication_info_array[i].linked_node);
			communication_info_array[i].linked_node = strdup(linked_node);
			communication_info_array[i].updated_time = updated_time;
			communication_info_array[i].tx = tx;
			communication_info_array[i].rx = rx;
			//communication_info_array[i].error_ratio_value = strdup(error_ratio_value);
			communication_info_array[i].current_round = current_round;

			communication_info_array[i].if_newest_flag= 1;//表示是最新的，还没有经过同步
			pthread_mutex_unlock(&communication_info_lock);
            return _SUCCESS;
        }
    }

	pthread_mutex_unlock(&communication_info_lock);
	return _ERROR;

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

				int current_round_;
				string_to_int(cJSON_GetObjectItem(communication_info_item, "current_round")->valuestring, &current_round_);	
				communication_info_array[j].current_round = current_round_; 

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

			int current_round_;
			string_to_int(cJSON_GetObjectItem(communication_info_item, "current_round")->valuestring, &current_round_); 
			communication_info_array[communication_info_cnt - 1].current_round = current_round_; 


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
	//printAllInfo();
	//printf("size: %d\n",size);
	
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


  		//printf("%d\n",i);
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
			cJSON_AddNumberToObject(eth_info, "port_id", array[i].eth_info.port_id); 
		}  
		cJSON_AddItemToObject(interface, "eth_info", eth_info);  


		cJSON *linked_eth_info = cJSON_CreateObject();  
		if (array[i].linked_eth_info.mac_addr != NULL) {  
		    cJSON_AddStringToObject(linked_eth_info, "ip_addr", array[i].linked_eth_info.ip_addr);  
			cJSON_AddStringToObject(linked_eth_info, "net_mask", array[i].linked_eth_info.net_mask);
		    cJSON_AddStringToObject(linked_eth_info, "mac_addr", array[i].linked_eth_info.mac_addr);  
			cJSON_AddStringToObject(linked_eth_info, "ip_name", array[i].linked_eth_info.ip_name);  
			cJSON_AddNumberToObject(linked_eth_info, "port_id", array[i].linked_eth_info.port_id); 
		}  
		cJSON_AddItemToObject(interface, "linked_eth_info", linked_eth_info); 

		//printf("%d\n",i);
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
  	//printf("%d\n",12312);
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


int update_interface_cnt_from_split_json(const char *filename)
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


int update_interface_cnt_from_overall_json(const char *current_node_name, const char *filename)
{
	//printf("current_node_name:%s filename: %s\n",current_node_name,filename);

	cJSON *json,*interfaces,*interface;
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

	interfaces = json;
	int cnt = 0;
	int _size = cJSON_GetArraySize(interfaces);
	//printf("xasxasx\n");
	for (int i = 0; i < _size; i++) {
	  interface = cJSON_GetArrayItem(interfaces, i);
		if (!interface) continue;

		//printf("%s\n",cJSON_GetObjectItem(interface, "located_node")->valuestring);

		/*这里只读取同属于一个节点下的接口，毕竟当前程序只在当前一个节点上跑*/
		if(strcmp(cJSON_GetObjectItem(interface, "located_node")->valuestring,current_node_name)!=0) continue; 

		cnt++;
	}
    return cnt;
}



int get_all_node_name_from_overall_json(const char *filename, const char *current_node_name, char *all_node_name[]) {  
    char buffer[BUFFER_SIZE];  
    FILE *file = fopen(filename, "r");  
    if (file == NULL) {  
        printf("Open failed\n");  
        return -1;  
    }  
    int i = 0;  
    while (i < BUFFER_SIZE - 1 && fscanf(file, "%c", &buffer[i]) == 1) {  
        i++;  
    }  
    if (i < BUFFER_SIZE) {  
        buffer[i] = '\0';  
    }  
    fclose(file);  
  
    cJSON *json = cJSON_Parse(buffer);  
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
  
    cJSON *interfaces = json;  
    int cnt = 0;  
    int _size = cJSON_GetArraySize(interfaces);  
    for (int i = 0; i < _size; i++) {  
        cJSON *interface = cJSON_GetArrayItem(interfaces, i);  
        if (!interface) continue;  


		int j=0;
  		for(;j<cnt;j++)
  		{
			if (strcmp(cJSON_GetObjectItem(interface, "located_node")->valuestring, all_node_name[j]) == 0) break; /*如果已经记录过了*/ 
  		}
		if(j<cnt)
		{
			continue;
		}
		//printf("%d\n",cnt);

        all_node_name[cnt] = strdup(cJSON_GetObjectItem(interface, "located_node")->valuestring); // 假设您想保存某个键的值  
        cnt++;  
    }  
  
    cJSON_Delete(json);  
    return cnt;  
}



int check_single_send_base_func_legal(const struct interface_info *info)
{
	char *send_func_name_array[] = {"send_packet_by_ip_addr", "send_packet_by_mac_addr", "send_packet_xy", "send_packet_rs485", "send_packet_can_fpu", "send_packet_can_gpu"};	
	int send_func_num = 6;
	

    for (int i = 0; i < send_func_num; i++) {  
        if (strcmp(info->base_send_func, send_func_name_array[i]) == 0) {  
            return _SUCCESS;
        }  
    }  
    return _ERROR;  
}

int check_all_send_base_func_legal(const struct interface_info *array,int size)
{

	
	for(int i=0;i<size;i++)
	{
		if(check_single_send_base_func_legal(&array[i])==_ERROR)
			return -i;
	}
	return _SUCCESS;
}


int check_single_recv_base_func_legal(const struct interface_info *info)
{
	char *recv_func_name_array[] = {"receive_packet_by_ip_addr", "receive_packet_by_mac_addr", "receive_packet_xy", "receive_packet_rs485", "receive_packet_can_fpu","receive_packet_can_gpu"};  
	int recv_func_num = 6;	

    for (int i = 0; i < recv_func_num; i++) {  
        if (strcmp(info->base_send_func, recv_func_name_array[i]) == 0) {  
            return _SUCCESS;
        }  
    }  
    return _ERROR;  
}

int check_all_recv_base_func_legal(const struct interface_info *array,int size)
{
	for(int i=0;i<size;i++)
	{
		if(check_single_recv_base_func_legal(&array[i])==_ERROR)
			return -i;
	}
	return _SUCCESS;
}


void    read_interface_info_array_from_split_json(const char *filename, struct interface_info *array) {
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


			cJSON *port_id_item = cJSON_GetObjectItem(tmp, "port_id");  
			if (port_id_item && cJSON_IsNumber(port_id_item)) {  
				array[i].eth_info.port_id = port_id_item->valuedouble;  
			} else {  
				array[i].eth_info.port_id = -1;	
			}  
			
		} else {  
			array[i].eth_info.ip_name = NULL;
			array[i].eth_info.ip_addr = NULL;
			array[i].eth_info.net_mask = NULL;
			array[i].eth_info.mac_addr = NULL;	
			array[i].eth_info.port_id = -1;
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

			cJSON *port_id_item = cJSON_GetObjectItem(tmp, "port_id");  
			if (port_id_item && cJSON_IsNumber(port_id_item)) {  
				array[i].linked_eth_info.port_id = port_id_item->valuedouble;  
			} else {  
				array[i].linked_eth_info.port_id = -1;	
			}  
			
		} else {  
			array[i].linked_eth_info.ip_name = NULL;
			array[i].linked_eth_info.ip_addr = NULL;
			array[i].linked_eth_info.net_mask = NULL;
			array[i].linked_eth_info.mac_addr = NULL;	
			array[i].linked_eth_info.port_id = -1;
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


void  read_interface_info_array_from_overall_json(const char *current_node_name, const char *filename, struct interface_info *array) {
	cJSON *json, *interfaces, *interface, *linked_interface, *tmp; 
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

	int cnt = 0;
	//printf("_size: %d\n",_size);
    for (int i = 0; i < _size; i++) {
        interface = cJSON_GetArrayItem(interfaces, i);
		if (!interface) continue;

		/*这里只读取同属于一个节点下的接口，毕竟当前程序只在当前一个节点上跑*/
		if(strcmp(cJSON_GetObjectItem(interface, "located_node")->valuestring,current_node_name)!=0) continue; 
		
		
		for(int j=0;j<_size;j++)/*检索当前节点所连接于的节点*/
		{
			linked_interface = cJSON_GetArrayItem(interfaces, j);
			if (!linked_interface) continue;
			if(strcmp(cJSON_GetObjectItem(interface, "linked_interface_name")->valuestring,cJSON_GetObjectItem(linked_interface, "interface_name")->valuestring)==0)
			{
				break;
			}
		}
		if (!linked_interface) 
		{
			printf("cannot find the linked interface for the interface \"%s\"\n[ERROR] loading terminate!\n",cJSON_GetObjectItem(interface, "interface_name")->valuestring);
			return;
		}

		array[cnt].initialized_flag = -1; //表示还未初始化

        // Allocate and fill located_node
        array[cnt].located_node = strdup(cJSON_GetObjectItem(interface, "located_node")->valuestring);
        // Similarly fill other fields...
        array[cnt].interface_name = strdup(cJSON_GetObjectItem(interface, "interface_name")->valuestring);
        array[cnt].interface_type = strdup(cJSON_GetObjectItem(interface, "interface_type")->valuestring);
        array[cnt].linked_node = strdup(cJSON_GetObjectItem(linked_interface, "located_node")->valuestring);
        array[cnt].linked_interface_name = strdup(cJSON_GetObjectItem(linked_interface, "interface_name")->valuestring);
        array[cnt].linked_interface_type = strdup(cJSON_GetObjectItem(linked_interface, "interface_type")->valuestring);
		array[cnt].center_interface_name = strdup(cJSON_GetObjectItem(interface, "center_interface_name")->valuestring);
		array[cnt].status_chooser = strdup(cJSON_GetObjectItem(interface, "status_chooser")->valuestring);
		array[cnt].base_send_func = strdup(cJSON_GetObjectItem(interface, "base_send_func")->valuestring);
		array[cnt].base_receive_func = strdup(cJSON_GetObjectItem(interface, "base_receive_func")->valuestring);
		array[cnt].msg_generator_of_sender = strdup(cJSON_GetObjectItem(interface, "msg_generator_of_sender")->valuestring);
		array[cnt].initializer_name = strdup(cJSON_GetObjectItem(interface, "initializer_name")->valuestring);
		array[cnt].closer_name = strdup(cJSON_GetObjectItem(interface, "closer_name")->valuestring);

        array[cnt].status = strdup(cJSON_GetObjectItem(interface, "status")->valuestring);
		array[cnt].mode = strdup(cJSON_GetObjectItem(interface, "mode")->valuestring);
		array[cnt].duplex = strdup(cJSON_GetObjectItem(interface, "duplex")->valuestring);


        // Handle nested structures
		// Fill eth_info  
		tmp = cJSON_GetObjectItem(interface, "eth_info");  
		if (tmp && cJSON_IsObject(tmp)) {  
			cJSON *ip_name_item = cJSON_GetObjectItem(tmp, "ip_name");  
			if (ip_name_item && cJSON_IsString(ip_name_item)) {  
				array[cnt].eth_info.ip_name = strdup(ip_name_item->valuestring);  
			} else {  
				array[cnt].eth_info.ip_name = NULL;	
			}  
			
			cJSON *ip_addr_item = cJSON_GetObjectItem(tmp, "ip_addr");  
			if (ip_addr_item && cJSON_IsString(ip_addr_item)) {  
				array[cnt].eth_info.ip_addr = strdup(ip_addr_item->valuestring);  
			} else {  
				array[cnt].eth_info.ip_addr = NULL;	
			}  

			cJSON *net_mask_item = cJSON_GetObjectItem(tmp, "net_mask");  
			if (net_mask_item && cJSON_IsString(net_mask_item)) {  
				array[cnt].eth_info.net_mask = strdup(net_mask_item->valuestring);  
			} else {  
				array[cnt].eth_info.net_mask = NULL;	
			}  

			cJSON *mac_addr_item = cJSON_GetObjectItem(tmp, "mac_addr");  
			if (mac_addr_item && cJSON_IsString(mac_addr_item)) {  
				array[cnt].eth_info.mac_addr = strdup(mac_addr_item->valuestring);  
			} else {  
				array[cnt].eth_info.mac_addr = NULL;	
			}  

			cJSON *port_id_item = cJSON_GetObjectItem(tmp, "port_id");  
			if (port_id_item && cJSON_IsNumber(port_id_item)) {  
				array[cnt].eth_info.port_id= port_id_item->valuedouble;  
			} else {  
				array[cnt].eth_info.port_id = -1;	
			}  
		} else {  
			array[cnt].eth_info.ip_name = NULL;
			array[cnt].eth_info.ip_addr = NULL;
			array[cnt].eth_info.net_mask = NULL;
			array[cnt].eth_info.mac_addr = NULL;	
			array[cnt].eth_info.port_id = -1;	
		}  

		// Fill linked_eth_info  
		tmp = cJSON_GetObjectItem(linked_interface, "eth_info");  
		if (tmp && cJSON_IsObject(tmp)) { 
			cJSON *ip_name_item = cJSON_GetObjectItem(tmp, "ip_name");  
			if (ip_name_item && cJSON_IsString(ip_name_item)) {  
				array[cnt].linked_eth_info.ip_name = strdup(ip_name_item->valuestring);  
			} else {  
				array[cnt].linked_eth_info.ip_name = NULL;	
			} 

			
			cJSON *ip_addr_item = cJSON_GetObjectItem(tmp, "ip_addr");  
			if (ip_addr_item && cJSON_IsString(ip_addr_item)) {  
				array[cnt].linked_eth_info.ip_addr = strdup(ip_addr_item->valuestring);  
			} else {  
				array[cnt].linked_eth_info.ip_addr = NULL;	
			}  

			cJSON *net_mask_item = cJSON_GetObjectItem(tmp, "net_mask");  
			if (net_mask_item && cJSON_IsString(net_mask_item)) {  
				array[cnt].linked_eth_info.net_mask = strdup(net_mask_item->valuestring);  
			} else {  
				array[cnt].linked_eth_info.net_mask = NULL;	
			}  

			cJSON *mac_addr_item = cJSON_GetObjectItem(tmp, "mac_addr");  
			if (mac_addr_item && cJSON_IsString(mac_addr_item)) {  
				array[cnt].linked_eth_info.mac_addr = strdup(mac_addr_item->valuestring);  
			} else {  
				array[cnt].linked_eth_info.mac_addr = NULL;	
			}  


			cJSON *port_id_item = cJSON_GetObjectItem(tmp, "port_id");  
			if (port_id_item && cJSON_IsNumber(port_id_item)) {  
				array[cnt].linked_eth_info.port_id= port_id_item->valuedouble;  
			} else {  
				array[cnt].linked_eth_info.port_id = -1;	
			}  
		} else {  
			array[cnt].linked_eth_info.ip_name = NULL;
			array[cnt].linked_eth_info.ip_addr = NULL;
			array[cnt].linked_eth_info.net_mask = NULL;
			array[cnt].linked_eth_info.mac_addr = NULL;		
			array[cnt].linked_eth_info.port_id = -1;	
		} 



		// Fill can_id	
		tmp = cJSON_GetObjectItem(interface, "can_info");  
		if (tmp && cJSON_IsObject(tmp)) {  
			cJSON *can_id_item = cJSON_GetObjectItem(tmp, "can_id");  
			if (can_id_item && cJSON_IsNumber(can_id_item)) {  
				array[cnt].can_info.can_id = can_id_item->valuedouble;  
			} else {  
				array[cnt].can_info.can_id = -1;  
			}  
			
			cJSON *baud_rate_item = cJSON_GetObjectItem(tmp, "baud_rate");  
			if (baud_rate_item && cJSON_IsNumber(baud_rate_item)) {  
				array[cnt].can_info.baud_rate = baud_rate_item->valuedouble;  
			} else {  
				array[cnt].can_info.baud_rate = -1;  
			} 
		} else {  
			array[cnt].can_info.can_id = -1;  
			array[cnt].can_info.baud_rate = -1;  
		}  


		// Fill linked_can_id	
		tmp = cJSON_GetObjectItem(linked_interface, "can_info");  
		if (tmp && cJSON_IsObject(tmp)) {  
			cJSON *can_id_item = cJSON_GetObjectItem(tmp, "can_id");  
			if (can_id_item && cJSON_IsNumber(can_id_item)) {  
				array[cnt].linked_can_info.can_id = can_id_item->valuedouble;  
			} else {  
				array[cnt].linked_can_info.can_id = -1;  
			} 

			cJSON *baud_rate_item = cJSON_GetObjectItem(tmp, "baud_rate");  
			if (baud_rate_item && cJSON_IsNumber(baud_rate_item)) {  
				array[cnt].linked_can_info.baud_rate = baud_rate_item->valuedouble;  
			} else {  
				array[cnt].linked_can_info.baud_rate = -1;  
			} 
		} else {  
			array[cnt].linked_can_info.can_id = -1;  
			array[cnt].linked_can_info.baud_rate = -1;  
		}



		// Fill baud_rate  
		tmp = cJSON_GetObjectItem(interface, "rs485_info");  
		if (tmp && cJSON_IsObject(tmp)) {  

			cJSON *rs485_dev_path_item = cJSON_GetObjectItem(tmp, "rs485_dev_path");	
			if (rs485_dev_path_item && cJSON_IsString(rs485_dev_path_item)) {  
				array[cnt].rs485_info.rs485_dev_path = strdup(rs485_dev_path_item->valuestring);  
			} else {  
				array[cnt].rs485_info.rs485_dev_path= NULL;  
			}  

			
			cJSON *rs485_gpio_number_item = cJSON_GetObjectItem(tmp, "rs485_gpio_number");	
			if (rs485_gpio_number_item && cJSON_IsNumber(rs485_gpio_number_item)) {  
				array[cnt].rs485_info.rs485_gpio_number = rs485_gpio_number_item->valuedouble;  
			} else {  
				array[cnt].rs485_info.rs485_gpio_number = -1;  
			}  

			
			cJSON *baud_rate_item = cJSON_GetObjectItem(tmp, "baud_rate");	
			if (baud_rate_item && cJSON_IsNumber(baud_rate_item)) {  
				array[cnt].rs485_info.baud_rate = baud_rate_item->valuedouble;  
			} else {  
				array[cnt].rs485_info.baud_rate = -1;  
			}  

			cJSON *databits_item = cJSON_GetObjectItem(tmp, "databits");
			if (databits_item && cJSON_IsNumber(databits_item)) {  
				array[cnt].rs485_info.databits = databits_item->valuedouble;  
			} else {  
				array[cnt].rs485_info.databits = -1;  
			}  

			cJSON *stopbits_item = cJSON_GetObjectItem(tmp, "stopbits");
			if (stopbits_item && cJSON_IsNumber(stopbits_item)) {  
				array[cnt].rs485_info.stopbits = stopbits_item->valuedouble;  
			} else {  
				array[cnt].rs485_info.stopbits = -1;  
			}  

			cJSON *paritybits_item = cJSON_GetObjectItem(tmp, "paritybits");
			if (paritybits_item && cJSON_IsNumber(paritybits_item)) {  
				array[cnt].rs485_info.paritybits = paritybits_item->valuedouble;  
			} else {  
				array[cnt].rs485_info.paritybits = 0;  
			}  

	
		} else {  
			array[cnt].rs485_info.rs485_dev_path = NULL; 
			array[cnt].rs485_info.rs485_gpio_number = -1; 
			array[cnt].rs485_info.baud_rate = -1;  
			array[cnt].rs485_info.databits = -1;  
			array[cnt].rs485_info.stopbits = -1;  
			array[cnt].rs485_info.paritybits = 0;  
		}


		// Fill linked_baud_rate  
		tmp = cJSON_GetObjectItem(linked_interface, "rs485_info");  
		if (tmp && cJSON_IsObject(tmp)) { 

			cJSON *rs485_dev_path_item = cJSON_GetObjectItem(tmp, "rs485_dev_path");	
			if (rs485_dev_path_item && cJSON_IsString(rs485_dev_path_item)) {  
				array[cnt].linked_rs485_info.rs485_dev_path = strdup(rs485_dev_path_item->valuestring);  
			} else {  
				array[cnt].linked_rs485_info.rs485_dev_path= NULL;  
			}  

			
			cJSON *rs485_gpio_number_item = cJSON_GetObjectItem(tmp, "rs485_gpio_number");	
			if (rs485_gpio_number_item && cJSON_IsNumber(rs485_gpio_number_item)) {  
				array[cnt].linked_rs485_info.rs485_gpio_number = rs485_gpio_number_item->valuedouble;  
			} else {  
				array[cnt].linked_rs485_info.rs485_gpio_number = -1;  
			}  
			
			cJSON *baud_rate_item = cJSON_GetObjectItem(tmp, "baud_rate");	
			if (baud_rate_item && cJSON_IsNumber(baud_rate_item)) {  
				array[cnt].linked_rs485_info.baud_rate = baud_rate_item->valuedouble;  
			} else {  
				array[cnt].linked_rs485_info.baud_rate = -1;  
			}  

			cJSON *databits_item = cJSON_GetObjectItem(tmp, "databits");
			if (databits_item && cJSON_IsNumber(databits_item)) {  
				array[cnt].linked_rs485_info.databits = databits_item->valuedouble;  
			} else {  
				array[cnt].linked_rs485_info.databits = -1;  
			}  

			cJSON *stopbits_item = cJSON_GetObjectItem(tmp, "stopbits");
			if (stopbits_item && cJSON_IsNumber(stopbits_item)) {  
				array[cnt].linked_rs485_info.stopbits = stopbits_item->valuedouble;  
			} else {  
				array[cnt].linked_rs485_info.stopbits = -1;  
			}  

			cJSON *paritybits_item = cJSON_GetObjectItem(tmp, "paritybits");
			if (paritybits_item && cJSON_IsNumber(paritybits_item)) {  
				array[cnt].linked_rs485_info.paritybits = paritybits_item->valuedouble;  
			} else {  
				array[cnt].linked_rs485_info.paritybits = 0;  
			}  

	
		} else {  
			array[cnt].linked_rs485_info.rs485_dev_path = NULL; 
			array[cnt].linked_rs485_info.rs485_gpio_number = -1; 
			array[cnt].linked_rs485_info.baud_rate = -1;  
			array[cnt].linked_rs485_info.databits = -1;  
			array[cnt].linked_rs485_info.stopbits = -1;  
			array[cnt].linked_rs485_info.paritybits = 0;  
		}

		cnt++;
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
	printf("***********************************start to free interface info array\n");
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
		//printf("%d\n",i);

		free(interface_info_array[i].eth_info.ip_name);
		free(interface_info_array[i].eth_info.ip_addr);
		free(interface_info_array[i].eth_info.net_mask);
		free(interface_info_array[i].eth_info.mac_addr);
		free(interface_info_array[i].eth_info.sock_addr);
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
	interface_cnt = 0;
	printf("***********************************interface info array being free!\n");
}

void free_communication_info_array()
{
	printf("***********************************start to free communication info array\n");
	for (int i = 0; i < communication_info_cnt; i++) {
		free(communication_info_array[i].linked_node);
		free(communication_info_array[i].interface_name);
		//free(communication_info_array[i].updated_time);
		//free(communication_info_array[i].error_ratio_value);
	}
	free(communication_info_array);
	communication_info_cnt=0;
	printf("***********************************communication info array being free!\n");
}


time_t get_test_begin_time()
{
	return test_begin_time;
}


void start_info_manager_from_split_json(const char *filename)
{
	printf("***********************************loading interface information......\n");

	int size = update_interface_cnt_from_split_json(filename);
	int ret = string_to_time_t(TEST_BEIGIN_TIME, &test_begin_time);
	
	if (ret!=_SUCCESS) {
		test_begin_time = 0;
		printf("cannot parse the test begin time!\n");
	}

	//free_interface_info_array();
	malloc_interface_info_array(size);
	read_interface_info_array_from_split_json(filename,interface_info_array);

	int count=0;
	for(int i=0;i<size;i++)
	{
		if(strcmp(interface_info_array[i].mode,"listen")==0)
			count++;
		if(strcmp(interface_info_array[i].interface_type,"eth")==0)
			interface_info_array[i].eth_info.sock_addr = (struct sockaddr_ll*)malloc(sizeof(struct sockaddr_ll));
	}


	printf("***********************************interface information loaded\n");



	ret = check_all_recv_base_func_legal(interface_info_array,size);
	if(ret<0)
	{
		printf("interface \"%s\"'s recv func is configured wrongly, which is %s\n",interface_info_array[-ret].interface_name,interface_info_array[-ret].base_receive_func);
		free_interface_info_array();
		return;
	}
	

	ret = check_all_send_base_func_legal(interface_info_array,size);
	if(ret<0)
	{
		printf("interface \"%s\"'s recv func is configured wrongly, which is %s\n",interface_info_array[-ret].interface_name,interface_info_array[-ret].base_send_func);
		free_interface_info_array();
		return;
	}
	

	
	printf("***********************************loading communication information......\n");
	initialize_communication_info_lock();



	//free_communication_info_array();
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

	printf("***********************************communication information loaded\n");
	
}


void start_info_manager_from_overall_json(const char* current_node_name, const char *filename)
{

	printf("***********************************loading interface information......\n");

    char *node_names[MAX_INTERFACES]; // 假设我们最多只关心10个接口  
    int node_cnt = get_all_node_name_from_overall_json(filename, current_node_name, node_names);  

	
	//printf("xas\n");

	for (int i = 0; i < node_cnt; i++) {  
		if (strcmp(node_names[i], current_node_name) == 0) {  
			break; // 找到匹配项，退出循环	
		}  
	  
		// 如果这是最后一个元素且没有找到匹配项  
		if (i == node_cnt - 1) {  
			printf("cannot find this node \"%s\"\n", current_node_name);  
			// 释放之前用 strdup 分配的内存  
			for (int j = 0; j < node_cnt; j++) {  
				free(node_names[j]);  
			}  
			return; // 提前退出函数  
		}  
	}  

	for (int j = 0; j < node_cnt; j++) {  
		free(node_names[j]);  
	} 


	int size = update_interface_cnt_from_overall_json(current_node_name,filename);
	int ret = string_to_time_t(TEST_BEIGIN_TIME, &test_begin_time);
	
	if (ret!=_SUCCESS) {
		test_begin_time = 0;
		printf("cannot parse the test begin time!\n");
	}

	//printf("size:%d\n",size);

	//free_interface_info_array();
	malloc_interface_info_array(size);
	read_interface_info_array_from_overall_json(current_node_name,filename,interface_info_array);
	printf("***********************************interface information loaded as below:\n\n\n");

	printAllInfo();
	printf("\n\n");

	
	printf("***********************************loading communication information......\n");
	initialize_communication_info_lock();
	
	int count=0;
	for(int i=0;i<size;i++)
	{
		if(strcmp(interface_info_array[i].mode,"listen")==0)
			count++;
		if(strcmp(interface_info_array[i].interface_type,"eth")==0)
			interface_info_array[i].eth_info.sock_addr = (struct sockaddr_ll*)malloc(sizeof(struct sockaddr_ll));
	}

	//printf("count:%d\n",count);

	//free_communication_info_array();
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


	printf("***********************************communication information loaded as below:\n\n\n");
	printAllCommucationInfo();
	printf("\n\n");
	
}


void reset_all_communication_infos(int rx,int tx)
{
	pthread_mutex_lock(&communication_info_lock);
	for(int i=0;i<communication_info_cnt;i++)
	{
		

					communication_info_array[i].updated_time = time(NULL);
					communication_info_array[i].tx = tx;
					communication_info_array[i].rx = rx;
					//communication_info_array[i].error_ratio_value = strdup(error_ratio_value);
					communication_info_array[i].current_round = -1;

					communication_info_array[i].if_newest_flag= 1;//表示是最新的，还没有经过同步	
	}
	pthread_mutex_unlock(&communication_info_lock);
}


void close_info_manager(const char *filename)
{
	if(filename!=NULL)/*看是否要存一下接口配置，存为一个针对当前节点的分分配置文件*/
		write_interface_info_array_to_json(filename,interface_info_array,interface_cnt);

	//printf("xsaxxsa\n");
	free_interface_info_array();
	free_communication_info_array();

	destroy_communication_info_lock();
}



int get_interface_cnt()
{
	return interface_cnt;
}


int get_port_id_by_index(int i)
{
	return interface_info_array[i].eth_info.port_id;
}

int get_linked_port_id_by_index(int i)
{
	return interface_info_array[i].linked_eth_info.port_id;
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

int get_temporary_sockfd_by_index(int i)
{
	return interface_info_array[i].eth_info.temporary_sockfd;
}
void set_temporary_sockfd_by_index(int i,int sockfd)
{
	interface_info_array[i].eth_info.temporary_sockfd = sockfd;
}

struct sockaddr_ll* get_sock_addr_value_addr(int i)
{
	return interface_info_array[i].eth_info.sock_addr;
}


void set_sock_addr_value_addr(int i,struct sockaddr_ll* addr)
{
	interface_info_array[i].eth_info.sock_addr = addr;
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

char* get_linked_ip_addr_by_index(int i)
{
	return interface_info_array[i].linked_eth_info.ip_addr;
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

char* get_mac_addr_by_index(int i) {
    return interface_info_array[i].eth_info.mac_addr;
}

char* get_linked_mac_addr_by_index(int i) {
    return interface_info_array[i].linked_eth_info.mac_addr;
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
	printf("Current Round: %d\n", info->current_round);
    printf("Listened Interface Name: %s\n", info->interface_name);  
	char time_buffer[80]; // Assuming enough space for ctime(3) format  
    time_t_to_string(info->updated_time,time_buffer,sizeof(time_buffer));
    printf("Updated Time: %s\n", time_buffer);  
    printf("Tx Number: %d\n", info->tx);  
	printf("Rx Number: %d\n", info->rx);  
	printf("\n"); // 添加空行以分隔不同的接口信息  
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
    }  
}

void printCertainCommucationInfo(const char* certain_interface_name)
{
    if (communication_info_array == NULL || communication_info_cnt <= 0) {  
        printf("communication info array is NULL or size is invalid\n");  
        return;  
    }  
  	printf("\n");
    for (int i = 0; i < communication_info_cnt; i++) {  
		if(strcmp(communication_info_array[i].interface_name,certain_interface_name)==0)
		{
        	print_communication_info(&communication_info_array[i]);
			return;
		}
    }  
	printf("cannot find the certain interface whose interface name is \"%s\"\n",certain_interface_name);
	return;
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
	printf("\tIP Name: %s\n", info->eth_info.ip_name); 
    printf("\tIP Address: %s\n", info->eth_info.ip_addr);  
    printf("\tNet Mask: %s\n", info->eth_info.net_mask);  
    printf("\tMAC Address: %s\n", info->eth_info.mac_addr);  
	printf("\tPort Id: %d\n", info->eth_info.port_id);  
  
    printf("Linked Ethernet Info:\n");  
	printf("\tIP Name: %s\n", info->linked_eth_info.ip_name); 
    printf("\tIP Address: %s\n", info->linked_eth_info.ip_addr);  
    printf("\tNet Mask: %s\n", info->linked_eth_info.net_mask);  
    printf("\tMAC Address: %s\n", info->linked_eth_info.mac_addr); 
	printf("\tPort Id: %d\n", info->linked_eth_info.port_id); 
  
    printf("CAN Info:\n");  
    printf("\tCAN ID: %d\n", info->can_info.can_id);  
	printf("\tCAN baud_rate: %d\n", info->linked_can_info.baud_rate);
  
    printf("Linked CAN Info:\n");  
    printf("\tCAN ID: %d\n", info->linked_can_info.can_id); 
	printf("\tCAN baud_rate: %d\n", info->linked_can_info.baud_rate);
  
    printf("RS485 Info:\n");  
	printf("\tRS485 Dev Path: %s\n", info->rs485_info.rs485_dev_path);  
    printf("\tGPIO Number: %d\n", info->rs485_info.rs485_gpio_number);  
    printf("\tData Bits: %d\n", info->rs485_info.databits);  
    printf("\tStop Bits: %d\n", info->rs485_info.stopbits);  
    printf("\tParity Bits: %u\n", info->rs485_info.paritybits);  
    printf("\tBaud Rate: %d\n", info->rs485_info.baud_rate);  
    printf("\tTemporary FD: %d\n", info->rs485_info.temporary_fd);  
  
    printf("Linked RS485 Info:\n");  
	printf("\tRS485 Dev Path: %s\n", info->linked_rs485_info.rs485_dev_path);  
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


