

#include "one_interface_operator.h"




void set_res_file_name(char *file_name)
{
	res_file_name=file_name;
}

void set_center_interface_name(char *interface_name)
{
	center_interface_name=interface_name;
}



int sync_communication_info(const char* center_interface_name)
{
	if(is_this_interface_in_current_node(center_interface_name))
	{
		write_communication_info_array_to_json(res_file_name);
	}else
	{
		// non_center_node reponsible for sending communication info to certer_node
		char* the_interface_linked_with_center_interface = get_interface_name_by_linked_interface_name(center_interface_name);
		if(!the_interface_linked_with_center_interface)
		{
			printf("sync failed because of missing the the interface linked with center interface!\n");
			return _ERROR;
		}else
		{
			char* communication_info_array_json_str = parse_communication_info_array_to_json();
			if(_ERROR == send_message(the_interface_linked_with_center_interface,communication_info_array_json_str))
			{
				printf("sync failed because of sending failing!\n");
				free(communication_info_array_json_str);
				return _ERROR;
			}
			free(communication_info_array_json_str);
		}	
	}
	return _SUCCESS;
}




int cnt = 0; // 全局计数器，需要线程安全  
pthread_mutex_t cnt_mutex = PTHREAD_MUTEX_INITIALIZER; // 互斥锁保护 cnt  
int round = 0;  


void deal_with_mnt(const char* linked_node,const char* listened_interface, const char* msg) {  
	//printf("%s\n",msg);
    if(strcmp(msg, "hello, are you here?") == 0)
    {
    	time_t current_time = time(NULL);  
        int current_round = (current_time - get_test_begin_time()) / MAX_WAITING_TIME_IN_ONE_ROUND;
		//printf("current_time: %d\n",current_time);
		//printf("test_begin_time: %d\n",get_test_begin_time());
		if(round == 0)
		{
			round = current_round;
			return ;
		}
		if (current_round > round) { 
			printf("current_round is %d\n",current_round);
            pthread_mutex_lock(&cnt_mutex);
			
			//在这个地方把上一轮统计结果传出去，要不直接就保存下来
			char *result = malloc(RESULT_STRING_SIZE);  
		    if (result == NULL) {  
		        // 处理内存分配失败的情况  
		        return NULL;  
		    }  
		    double ratio = 1 - (double)cnt / PAKCAGES_NUM_ONE_TIME;  
		    snprintf(result, RESULT_STRING_SIZE, "%.2f", ratio); 
			update_communication_info_array(linked_node,listened_interface,time(NULL),PAKCAGES_NUM_ONE_TIME,cnt);
			printf("the interface \"%s\" got an error ratio value with %s ( tx:%d rx:%d)\n",listened_interface,result,PAKCAGES_NUM_ONE_TIME,cnt); 
			free(result);	

			sync_communication_info(center_interface_name);


            cnt = 1; // 重置计数器  
            round = current_round; 
            pthread_mutex_unlock(&cnt_mutex);  
        }else{
			pthread_mutex_lock(&cnt_mutex); 
	        cnt++;  
			pthread_mutex_unlock(&cnt_mutex);   
		}
		return;
    }else if(update_communication_info_array_from_json(msg)==_SUCCESS)
    {
    	write_communication_info_array_to_json(res_file_name);
		return;
    }
	printf("msg is wrong!\n");
	printf("msg: %s\n",msg);
}  




  
void listen_upon_one_interface_in_one_time(char *linked_node, char *listened_interface) {   
        receive_message(linked_node,listened_interface, deal_with_mnt, MAX_WAITING_TIME_IN_ONE_ROUND);  
}  
  



void *thread_function(void *args) {  
    ThreadArgs *ta = (ThreadArgs *)args;  
	send_message(ta->test_interface, ta->message);
    return NULL;  
}  



void test_upon_one_interface_in_one_time(const char *test_interface,const char *message,int packages_num)
{  
	time_t current_time = time(NULL);  
	int current_round = (current_time - get_test_begin_time()) / MAX_WAITING_TIME_IN_ONE_ROUND; 
	//printf("current_time: %d\n",current_time);
	//printf("test_begin_time: %d\n",get_test_begin_time());
	if(round == 0)
	{
		round = current_round;
		return ;
	}
	if (current_round > round) { 
		printf("current round is %d\n",current_round);
		pthread_t threads[packages_num];  
	    ThreadArgs args[packages_num];  
		struct timespec delay;
		delay.tv_sec = 0;  // 秒      
		delay.tv_nsec = SENDING_TIME_SPEC;  // 100毫秒 = 100,000,000纳秒  
	  
	    for (int i = 0; i < packages_num; i++) {  
	        args[i].test_interface = test_interface;  
	        args[i].message = message;  
			nanosleep(&delay, NULL);
	        pthread_create(&threads[i], NULL, thread_function, &args[i]);  
	    }  
	  
	    for (int i = 0; i < packages_num; i++) {  
	        pthread_join(threads[i], NULL);  
	    }  
		round=current_round;
	}
}  


#if 0
int main() {  
    
    return 0;  
}

#endif



