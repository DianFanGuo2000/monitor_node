#include "one_interface_operator.h"

char* generate_reply(const char* msg) {  
	printf("%s\n",msg);
    if(strcmp(msg, "hello, are you here?") == 0)
    {
    	return "yes, i am here!";  
    }
    else if(update_communication_info_array_from_json(msg)==_SUCCESS)
    {
    	write_communication_info_array_to_json(res_file_name);
    	return "ok";
    }
	printf("msg is wrong!\n");
	return NULL; // ����������������������������Ϊ�յ������ݰ�����Ч���ݰ�
}  

void set_res_file_name(char *file_name)
{
	res_file_name=file_name;
}


  
void listen_upon_one_interface(char *listened_interface) {  
   	receive_and_reply(listened_interface,  generate_reply, 2);
   	// ��������һЩ��ʱ������������������� CPU ʹ����  
    // sleep(1); // ����һ�루��Ҫ���� unistd.h��  
}  
  


int cnt = 0; // ȫ�ּ���������Ҫ�̰߳�ȫ  
pthread_mutex_t cnt_mutex = PTHREAD_MUTEX_INITIALIZER; // ���������� cnt  
  
void *thread_function(void *args) {  
    ThreadArgs *ta = (ThreadArgs *)args;  
	//pthread_mutex_lock(&cnt_mutex);
	int flag = send_until_being_replied(ta->test_interface, ta->message,ta->max_waiting_time_in_one_sending_time,ta->max_waiting_time_in_all, 1, ta->expected_reply_str);
	//pthread_mutex_unlock(&cnt_mutex);  
    if (_ERROR != flag) // if a reply is got, the send in this time is regarded as a successful send test  
    {  
    	pthread_mutex_lock(&cnt_mutex); 
        cnt++;  
		pthread_mutex_unlock(&cnt_mutex);  
    }  
    return NULL;  
}  
  
char *test_upon_one_interface_threaded(const char *test_interface,  
                                      const char *message, 
                                      long max_waiting_time_in_one_sending_time, 
                             		  long max_waiting_time_in_all,
                                      int packages_num,  
                                      const char *expected_reply_str) {  
    pthread_t threads[packages_num];  
    ThreadArgs args[packages_num];  
	cnt = 0;
	struct timespec delay;
	delay.tv_sec = 0;  // ��      
	delay.tv_nsec = 200000000;  // 100���� = 100,000,000����  
  
    for (int i = 0; i < packages_num; i++) {  
        args[i].test_interface = test_interface;  
        args[i].message = message;  
		args[i].max_waiting_time_in_one_sending_time = max_waiting_time_in_one_sending_time;  
        args[i].max_waiting_time_in_all = max_waiting_time_in_all;  
        args[i].expected_reply_str = expected_reply_str;  
		nanosleep(&delay, NULL);
        pthread_create(&threads[i], NULL, thread_function, &args[i]);  
    }  
  
    for (int i = 0; i < packages_num; i++) {  
        pthread_join(threads[i], NULL);  
    }  
  
    char *result = malloc(RESULT_STRING_SIZE);  
    if (result == NULL) {  
        // �����ڴ����ʧ�ܵ����  
        return NULL;  
    }  
    double ratio = 1 - (double)cnt / packages_num;  
    snprintf(result, RESULT_STRING_SIZE, "%.2f", ratio);  
  
    return result; // ��������Ҫ�����ͷ��ڴ�  
}  


#if 0
int main() {  
    
    return 0;  
}

#endif



