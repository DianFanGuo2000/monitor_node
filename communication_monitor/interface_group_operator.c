
#include "interface_group_operator.h"


void test_upon_interface_group() {  
	int cnt = get_interface_cnt();  
    int i;  
    pid_t pid;  
    int status;  
  
    for (i = 0; i < cnt; i++) {  
        while (1) {  // 使用无限循环来确保在异常退出时重新创建子进程  
            pid = fork();  
  
            if (pid == 0) {  
                // 子进程  
                char *res;
				res = test_upon_one_interface_threaded(get_interface_name(i), "hello, are you here?", MAX_WAITING_TIME_IN_ONE_SENDING_TIME,MAX_WAITING_TIME_IN_ALL, PAKCAGES_NUM_ONE_TIME, "yes, i am here!");  
				update_communication_info_array(get_interface_name(i),time(NULL),res);
				printf("the interface \"%s\" got an error ratio value with %s\n",get_interface_name(i),res); 
				free(res);  
                exit(0); // 子进程完成后退出  
            } else if (pid < 0) {  
                // fork 失败  
                perror("fork failed");  
                sleep(1);  // 短暂等待后重试  
                continue;  // 回到循环开始，重新尝试fork  
            }  
  
            // 父进程继续，或者在这里等待子进程  
            pid_t wpid = waitpid(pid, &status, 0);  
            if (wpid == -1) {  
                perror("waitpid failed");  
                continue;  // 跳过当前循环，不重新fork  
            }  
  
            if (WIFEXITED(status)) {  
                if (WEXITSTATUS(status) != 0) {  
                    // 子进程异常退出  
                    printf("Child %d exited abnormally with status %d, restarting...\n", pid, WEXITSTATUS(status));  
                    continue;  // 回到循环开始，重新fork  
                }  
            } else if (WIFSIGNALED(status)) {  
                // 子进程被信号杀死  
                printf("Child %d killed by signal %d, restarting...\n", pid, WTERMSIG(status));  
                continue;  // 回到循环开始，重新fork  
            }  
  
            // 如果子进程正常退出，则跳出循环  
            break;  
        }  
    }  
  
    // 所有子进程（或它们的重启尝试）都已完成  
    while (wait(NULL) > 0);  
}  




void listen_upon_interface_group() {  
    int cnt = get_interface_cnt();  
    int i;  
    pid_t pid;  
    int status;  
  
    for (i = 0; i < cnt; i++) {  
        while (1) {  // 使用无限循环来确保在异常退出时重新创建子进程  
            pid = fork();  
  
            if (pid == 0) {  
                // 子进程  
                listen_upon_one_interface(get_interface_name(i));  
                exit(0); // 子进程完成后退出  
            } else if (pid < 0) {  
                // fork 失败  
                perror("fork failed");  
                sleep(1);  // 短暂等待后重试  
                continue;  // 回到循环开始，重新尝试fork  
            }  
  
            // 父进程继续，或者在这里等待子进程  
            pid_t wpid = waitpid(pid, &status, 0);  
            if (wpid == -1) {  
                perror("waitpid failed");  
                continue;  // 跳过当前循环，不重新fork  
            }  
  
            if (WIFEXITED(status)) {  
                if (WEXITSTATUS(status) != 0) {  
                    // 子进程异常退出  
                    printf("Child %d exited abnormally with status %d, restarting...\n", pid, WEXITSTATUS(status));  
                    continue;  // 回到循环开始，重新fork  
                }  
            } else if (WIFSIGNALED(status)) {  
                // 子进程被信号杀死  
                printf("Child %d killed by signal %d, restarting...\n", pid, WTERMSIG(status));  
                continue;  // 回到循环开始，重新fork  
            }  
  
            // 如果子进程正常退出，则跳出循环  
            break;  
        }  
    }  
  
    // 所有子进程（或它们的重启尝试）都已完成  
    while (wait(NULL) > 0);  
}  


int sync_communication_info(const char* center_interface_name)
{
	if(is_this_interface_in_current_node(center_interface_name))
	{
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
		}	
	}	
}


#if 1
int main(int argc, char *argv[]) {
    if (argc != 4) {  
        fprintf(stderr, "Usage: %s <config_file> <mode> [<center_node_name>( when test mode)、<res_file_name>( when listen mode)]\n", argv[0]);  
        fprintf(stderr, "Mode should be 'test' or 'listen'.\n");  
        return 1; // 表示程序因为错误的参数而退出  
    }  
	
  
    const char* config_file = argv[1];  
    const char* mode = argv[2];
	char msg[MAX_MSG_LEN];
  
    if (strcmp(mode, "test") == 0) {  
        start_and_load_info(config_file); 
		// 下面开始循环测试各个配置好的物理通信接口
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
		// 陪试不需要同步测试结果
    } else {  
        fprintf(stderr, "Invalid mode '%s'. Valid modes are 'test' 'listen'.\n", mode);  
        return 1; // 表示程序因为无效的模式而退出  
    }
		
    return 0; // 表示程序正常退出  
}

#endif



#if 0
int main(int argc, char *argv[]) {  
   	start_and_load_info("configure1.json");
	printAllInfo();
    return 0; // 表示程序正常退出  
}

#endif






