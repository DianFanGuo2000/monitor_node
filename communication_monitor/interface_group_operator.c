
#include "interface_group_operator.h"


void test_upon_interface_group() {  
	int cnt = get_interface_cnt();  
    int i;  
    pid_t pid;  
    int status;  
  
    for (i = 0; i < cnt; i++) {  
        while (1) {  // ʹ������ѭ����ȷ�����쳣�˳�ʱ���´����ӽ���  
            pid = fork();  
  
            if (pid == 0) {  
                // �ӽ���  
                char *res;
				res = test_upon_one_interface_threaded(get_interface_name(i), "hello, are you here?", MAX_WAITING_TIME_IN_ONE_SENDING_TIME,MAX_WAITING_TIME_IN_ALL, PAKCAGES_NUM_ONE_TIME, "yes, i am here!");  
				update_communication_info_array(get_interface_name(i),time(NULL),res);
				printf("the interface \"%s\" got an error ratio value with %s\n",get_interface_name(i),res); 
				free(res);  
                exit(0); // �ӽ�����ɺ��˳�  
            } else if (pid < 0) {  
                // fork ʧ��  
                perror("fork failed");  
                sleep(1);  // ���ݵȴ�������  
                continue;  // �ص�ѭ����ʼ�����³���fork  
            }  
  
            // �����̼���������������ȴ��ӽ���  
            pid_t wpid = waitpid(pid, &status, 0);  
            if (wpid == -1) {  
                perror("waitpid failed");  
                continue;  // ������ǰѭ����������fork  
            }  
  
            if (WIFEXITED(status)) {  
                if (WEXITSTATUS(status) != 0) {  
                    // �ӽ����쳣�˳�  
                    printf("Child %d exited abnormally with status %d, restarting...\n", pid, WEXITSTATUS(status));  
                    continue;  // �ص�ѭ����ʼ������fork  
                }  
            } else if (WIFSIGNALED(status)) {  
                // �ӽ��̱��ź�ɱ��  
                printf("Child %d killed by signal %d, restarting...\n", pid, WTERMSIG(status));  
                continue;  // �ص�ѭ����ʼ������fork  
            }  
  
            // ����ӽ��������˳���������ѭ��  
            break;  
        }  
    }  
  
    // �����ӽ��̣������ǵ��������ԣ��������  
    while (wait(NULL) > 0);  
}  




void listen_upon_interface_group() {  
    int cnt = get_interface_cnt();  
    int i;  
    pid_t pid;  
    int status;  
  
    for (i = 0; i < cnt; i++) {  
        while (1) {  // ʹ������ѭ����ȷ�����쳣�˳�ʱ���´����ӽ���  
            pid = fork();  
  
            if (pid == 0) {  
                // �ӽ���  
                listen_upon_one_interface(get_interface_name(i));  
                exit(0); // �ӽ�����ɺ��˳�  
            } else if (pid < 0) {  
                // fork ʧ��  
                perror("fork failed");  
                sleep(1);  // ���ݵȴ�������  
                continue;  // �ص�ѭ����ʼ�����³���fork  
            }  
  
            // �����̼���������������ȴ��ӽ���  
            pid_t wpid = waitpid(pid, &status, 0);  
            if (wpid == -1) {  
                perror("waitpid failed");  
                continue;  // ������ǰѭ����������fork  
            }  
  
            if (WIFEXITED(status)) {  
                if (WEXITSTATUS(status) != 0) {  
                    // �ӽ����쳣�˳�  
                    printf("Child %d exited abnormally with status %d, restarting...\n", pid, WEXITSTATUS(status));  
                    continue;  // �ص�ѭ����ʼ������fork  
                }  
            } else if (WIFSIGNALED(status)) {  
                // �ӽ��̱��ź�ɱ��  
                printf("Child %d killed by signal %d, restarting...\n", pid, WTERMSIG(status));  
                continue;  // �ص�ѭ����ʼ������fork  
            }  
  
            // ����ӽ��������˳���������ѭ��  
            break;  
        }  
    }  
  
    // �����ӽ��̣������ǵ��������ԣ��������  
    while (wait(NULL) > 0);  
}  


int sync_communication_info(const char* center_interface_name)
{
	if(is_this_interface_in_current_node(center_interface_name))
	{
        return _SUCCESS;
	}
    else
	{
		// non_center_node reponsible for sending communication info to certer_node
		char* the_interface_linked_with_center_interface = get_interface_name_by_linked_interface_name(center_interface_name);
		if(!the_interface_linked_with_center_interface)
		{
			printf("sync failed because of missing the the interface linked with center interface!\n");
			return _ERROR;
		}
        else
		{
			//printf("%s\n",the_interface_linked_with_center_interface);
			char* communication_info_array_json_str = parse_communication_info_array_to_json();
			//printf("%s\n",communication_info_array_json_str);
			if(_ERROR == send_until_being_replied(the_interface_linked_with_center_interface,communication_info_array_json_str,SYNC_TIME_IN_ONE_TIME,SYNC_TIME_IN_ALL,5,"ok"))
			{
				printf("sync failed because of sending failing!\n");
				free(communication_info_array_json_str);
				return _ERROR;
			}
			free(communication_info_array_json_str);
            return _ERROR;
		}	
	}	
}


#if 1
int main(int argc, char *argv[]) {
    if (argc != 4) {  
        fprintf(stderr, "Usage: %s <config_file> <mode> [<center_node_name>( when test mode)��<res_file_name>( when listen mode)]\n", argv[0]);  
        fprintf(stderr, "Mode should be 'test' or 'listen'.\n");  
        return 1; // ��ʾ������Ϊ����Ĳ������˳�  
    }  
	
  
    const char* config_file = argv[1];  
    const char* mode = argv[2];
	//char msg[MAX_MSG_LEN];
  
    if (strcmp(mode, "test") == 0) {  
        start_and_load_info(config_file); 
		// ���濪ʼѭ�����Ը������úõ�����ͨ�Žӿ�
		while(1)
		{
			test_upon_interface_group();
			sync_communication_info(argv[3]);
		}
    } else if (strcmp(mode, "listen") == 0) {
        start_and_load_info(config_file);
		set_res_file_name(argv[3]);
		while(1){
			listen_upon_interface_group(); 
		}
		// ���Բ���Ҫͬ�����Խ��
    } else {  
        fprintf(stderr, "Invalid mode '%s'. Valid modes are 'test' 'listen'.\n", mode);  
        return 1; // ��ʾ������Ϊ��Ч��ģʽ���˳�  
    }
		
    return 0; // ��ʾ���������˳�  
}

#endif



#if 0
int main(int argc, char *argv[]) {  
   	start_and_load_info("configure1.json");
	printAllInfo();
    return 0; // ��ʾ���������˳�  
}

#endif






