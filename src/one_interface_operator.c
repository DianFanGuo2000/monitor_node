

#include "one_interface_operator.h"




void set_res_file_name(char *file_name)
{
	res_file_name=file_name;
}





void update_status_in_current_round(const char *updated_interface,const char *mode,int current_round) {  
    // 调用choose函数获取状态索引，并更新状态  
    int index = get_interface_index(updated_interface);
	char *status_chooser_name_for_updated_interface = get_status_chooser_by_index(index);
    char* status_res = status_chooser_transfer(status_chooser_name_for_updated_interface,updated_interface,mode,current_round);  
    set_status(updated_interface,status_res); 
	//printf("now %s 's status is %s\n",updated_interface,status_res);
}  


/*遵循一个原则，只发送自己所监听的部分，而不是所有部分*/
int sync_communication_info(const char* listened_interface, const char* center_interface_name)
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
			char* communication_info_array_json_str = parse_communication_info_array_with_certain_listen_interface_to_json(listened_interface);
			//printf("communication_info_array_json_str:%s\n",communication_info_array_json_str);
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




// Global flag indicating whether initialization has occurred  
int initialized_flag = -1;  
  
// Global counter array, needs to be thread-safe  
int *cnt_array = NULL;  
  
// Array of mutexes to protect cnt_array  
pthread_mutex_t *cnt_mutex_array = NULL;  
  
// Assuming round_array should be an array as well, initialized to 0  
int *round_array = NULL;  

// Mutex to protect the initialization process  
pthread_mutex_t init_mutex = PTHREAD_MUTEX_INITIALIZER; 


// Initialization function  
void init_test_or_listen_record_arrays() {  
    pthread_mutex_lock(&init_mutex); // Lock the mutex before initialization  

	printf("recording variables being initialized\n");
  
    if (initialized_flag == -1) {  
        int number = get_interface_cnt();  
  
        // Allocate memory for arrays  
        cnt_array = malloc(number * sizeof(int));  
        if (cnt_array == NULL) {  
            // Handle error  
            pthread_mutex_unlock(&init_mutex); // Unlock the mutex on error  
            return;  
        }  
		for(int i=0;i<number;i++)
			cnt_array[i]=0;
  
        cnt_mutex_array = malloc(number * sizeof(pthread_mutex_t));  
        if (cnt_mutex_array == NULL) {  
            // Cleanup cnt_array and handle error  
            free(cnt_array);  
            pthread_mutex_unlock(&init_mutex); // Unlock the mutex on error  
            return;  
        }  
  
        // Initialize mutexes  
        for (int i = 0; i < number; i++) {  
            pthread_mutex_init(&cnt_mutex_array[i], NULL);  
        }  
  
        // Assuming round_array should also be an array, initialized to 0  
        round_array = malloc(number * sizeof(int)); 
        if (round_array == NULL) {  
            // Cleanup cnt_array, cnt_mutex_array, and handle error  
            free(cnt_array);  
            for (int i = 0; i < number; i++) {  
                pthread_mutex_destroy(&cnt_mutex_array[i]);  
            }  
            free(cnt_mutex_array);  
            pthread_mutex_unlock(&init_mutex); // Unlock the mutex on error  
            return;  
        }  
        // Initialize round_array to 0  
        for (int i = 0; i < number; i++) {  
            round_array[i] = 0;  
        }  
  
        // Mark as initialized  
        initialized_flag = 0;  
    }  

	printf("recording variables initialized done\n");
    pthread_mutex_unlock(&init_mutex); // Unlock the mutex after initialization  
}  


void free_test_or_listen_record_arrays() {  
	pthread_mutex_lock(&init_mutex); // Lock the mutex before initialization 
	printf("recording variables being free\n");
    // 假设initialized_flag和init_mutex是全局的，并且我们知道它们的状态  
    // 如果需要，可以在这里添加检查以确保在释放资源之前已经正确初始化  
  
    // 如果数组和互斥锁已经初始化，则释放它们  
    if (initialized_flag == 0) {  
        // 释放round_array  
        free(round_array);  
        round_array = NULL; // 可选，将指针置为NULL以避免野指针  
  
        // 销毁cnt_mutex_array中的每个互斥锁并释放内存  
        for (int i = 0; i < get_interface_cnt(); i++) { // 注意：这里假设get_interface_cnt()在释放时仍然有效  
            pthread_mutex_destroy(&cnt_mutex_array[i]);  
        }  
        free(cnt_mutex_array);  
        cnt_mutex_array = NULL; // 可选  
  
        // 释放cnt_array  
        free(cnt_array);  
        cnt_array = NULL; // 可选  
  
        // 标记为未初始化（可选，取决于你的具体需求）  
        initialized_flag = -1;  
    }  

	printf("recording variables free done\n");
	pthread_mutex_unlock(&init_mutex); // Unlock the mutex after initialization  

}  

void deal_with_mnt(const char* linked_node,const char* listened_interface, const char* msg) {  
	//printf("linked_node:%s listened_interface:%s msg:%s \n",linked_node,listened_interface,msg);
	int ind = get_interface_index(listened_interface);
	time_t current_time = time(NULL);  
    int current_round = (current_time - get_test_begin_time()) / MAX_WAITING_TIME_IN_ONE_ROUND;

	pthread_mutex_lock(&cnt_mutex_array[ind]);

	/*复原一下发送方发送的字符串*/
	char expect_msg[MAX_MSG_LEN];
	int ret = msg_generator_transfer(get_msg_generator_of_sender_by_index(ind),get_linked_interface_name_by_index(ind),\
		get_interface_mode_by_index(ind),current_round,expect_msg);
	if(ret<0)
	{
		printf("cannot recover the sended msg for listened_interface: %s!\n",listened_interface);
		return;
	}
	
	if(strcmp(msg, expect_msg) == 0)
    {

		//printf("current_time: %d\n",current_time);
		//printf("test_begin_time: %d\n",get_test_begin_time());
		
		if(round_array[ind] == 0)
		{
			cnt_array[ind] = 1; // 重置计数器 
			round_array[ind] = current_round;
			pthread_mutex_unlock(&cnt_mutex_array[ind]);
			return ;
		}


		if (current_round > round_array[ind]) {
			printf("round changed! ");
			printf("current_round is %d, current listened interface is %s, the cnt of this interface after this listen time is %d\n",current_round,listened_interface,1);
		}else
		{
			printf("round stay the same! ");
			printf("current_round is %d, current listened interface is %s, the cnt of this interface after this listen time is %d\n",current_round,listened_interface,cnt_array[ind]+1);
		}


		if (current_round > round_array[ind]) { 
            //pthread_mutex_lock(&cnt_mutex_array[ind]);
			
			//在这个地方把上一轮统计结果传出去，要不直接就保存下来
			char *result = malloc(RESULT_STRING_SIZE);  
		    if (result == NULL) {  
		        // 处理内存分配失败的情况  
				pthread_mutex_unlock(&cnt_mutex_array[ind]);   
				return NULL;  
		    }  

			/*
		    double ratio = 1 - (double)cnt_array[ind]/ PAKCAGES_NUM_ONE_TIME;  
		    snprintf(result, RESULT_STRING_SIZE, "%.2f", ratio); 
			printf("the interface \"%s\" got an error ratio value with %s ( tx:%d rx:%d)\n",listened_interface,result,PAKCAGES_NUM_ONE_TIME,cnt_array[ind]); 
			free(result);	*/

			update_communication_info_array(linked_node,listened_interface,time(NULL),PAKCAGES_NUM_ONE_TIME,cnt_array[ind]);

			printf("\n");
			printf("now sync the listened result of last round( %d ), current listened interface is %s, the result is below:\n",round_array[ind],listened_interface);
			printAllCommucationInfo();
			printf("\n");
			
			//printf("listened_interface get_interface_status(listened_interface) current_round: %s %s %d\n",listened_interface,get_interface_status(listened_interface),current_round);


			sync_communication_info(listened_interface,get_center_interface_name(ind));


            cnt_array[ind] = 1; // 重置计数器  
            round_array[ind]= current_round; 
            //pthread_mutex_unlock(&cnt_mutex_array[ind]);  
        }else{
			//pthread_mutex_lock(&cnt_mutex_array[ind]); 
	        cnt_array[ind]++;  
			//printf("listened_interface get_interface_status(listened_interface) : %s %s\n",listened_interface,get_interface_status(listened_interface));
			//pthread_mutex_unlock(&cnt_mutex_array[ind]);   
		}
		pthread_mutex_unlock(&cnt_mutex_array[ind]);   
		return;
    }else if(update_communication_info_array_from_json(msg)==_SUCCESS)
    {
    	write_communication_info_array_to_json(res_file_name);
		pthread_mutex_unlock(&cnt_mutex_array[ind]);   
		return;
    }
	pthread_mutex_unlock(&cnt_mutex_array[ind]);   
	printf("msg is wrong!\n");
	printf("msg: %s  expect_msg:%s \n",msg,expect_msg);
}  





void listen_upon_one_interface_in_one_time(char *linked_node, char *listened_interface) {  
    time_t current_time = time(NULL);  
    int current_round = (current_time - get_test_begin_time()) / MAX_WAITING_TIME_IN_ONE_ROUND;  
    update_status_in_current_round(listened_interface,"listen", current_round);  

	receive_message(linked_node,listened_interface, deal_with_mnt, MAX_WAITING_TIME_IN_ONE_ROUND); 

}  
  




void *test_thread_function(void *arg) {  
    ThreadArgs *ta = (ThreadArgs *)arg;  
	send_message(ta->interface_name, ta->message);
    return NULL;  
}  



void test_upon_one_interface_in_one_time(const char *test_interface,int packages_num)
{  

	time_t current_time = time(NULL);  
	int current_round = (current_time - get_test_begin_time()) / MAX_WAITING_TIME_IN_ONE_ROUND; 
	//printf("current_time: %d\n",current_time);
	//printf("test_begin_time: %d\n",get_test_begin_time());

	int ind = get_interface_index(test_interface);

	char message[MAX_MSG_LEN];
	int ret = msg_generator_transfer(get_msg_generator_of_sender_by_index(ind),get_interface_name_by_index(ind),"test",current_round,message);
	if(ret<0)
	{
		printf("cannot generate the sended msg for test_interface: %s!\n",test_interface);
		return ;
	}

	
	if(round_array[ind] == 0)
	{
		round_array[ind] = current_round;
		return ;
	}

	if (current_round > round_array[ind]) { 
		printf("current round is %d\n",current_round);
		update_status_in_current_round(test_interface,"test",current_round);
		
		pthread_t threads[packages_num];  
	    ThreadArgs args[packages_num];  
		struct timespec delay;
		delay.tv_sec = 0;  // 秒      
		delay.tv_nsec = WAITING_TIME_FOR_SENDING_AFTER_ONE_ROUND_CHANGING_POINT;  // 100毫秒 = 100,000,000纳秒  
		nanosleep(&delay, NULL);
		
		delay.tv_sec = 0;  // 秒      
		delay.tv_nsec = SENDING_TIME_SPEC;  // 100毫秒 = 100,000,000纳秒  
		
	    for (int i = 0; i < packages_num; i++) {  
	        args[i].interface_name = test_interface;  
	        args[i].message = message;  
			nanosleep(&delay, NULL);
	        pthread_create(&threads[i], NULL, test_thread_function, &args[i]);  

			char *interface_status = get_interface_status(test_interface);
			if(strcmp(interface_status, "receiving") == 0 || strcmp(interface_status, "closed") == 0 )
			{
				continue;
			}else
			{
				printf("now sending the package %d through the interface \"%s\"\n",i+1,test_interface);
			}
		}  
	  
	    for (int i = 0; i < packages_num; i++) {  
	        pthread_join(threads[i], NULL);  
	    }  
		round_array[ind]=current_round;
	}
}  


#if 0
int main() {  
    
    return 0;  
}

#endif



