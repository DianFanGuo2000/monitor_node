
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
				while(1)
					test_upon_one_interface_in_one_time(get_interface_name(i), "hello, are you here?", PAKCAGES_NUM_ONE_TIME);  
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
                while(1)
                	listen_upon_one_interface_in_one_time(get_linked_node(i),get_interface_name(i));  
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





#if 1
int main(int argc, char *argv[]) {
    if (argc != 5 && argc != 3) {  
        fprintf(stderr, "Usage: %s <config_file> <mode> [ <center_interface_name> <res_file_name> (when listen mode)]\n", argv[0]);  
        fprintf(stderr, "Mode should be 'test' or 'listen'.\n");  
        return 1; // 表示程序因为错误的参数而退出  
    }  
  
    const char* config_file = argv[1];  
    const char* mode = argv[2];

	

    if (strcmp(mode, "test") == 0) {  
        start_and_load_info(config_file); 
		// 下面开始循环测试各个配置好的物理通信接口
		test_upon_interface_group();
    } else if (strcmp(mode, "listen") == 0) {
        start_and_load_info(config_file);
		set_center_interface_name(argv[3]);
		set_res_file_name(argv[4]);
		listen_upon_interface_group();
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
    time_t parsed_time = time(NULL);  // 获取当前时间  
    char time_str[80];  
  
    // 将 time_t 转换为字符串  
    if (time_t_to_string(parsed_time, time_str, sizeof(time_str)) == _SUCCESS) {  
        printf("Converted time to string: %s\n", time_str);  
  
        // 使用一个已知有效的时间字符串来测试 string_to_time_t  
        const char* test_time_buffer = "Tue Mar  3 10:03:58 2020"; // 示例时间字符串  
  
        // 将字符串转换回 time_t  
        if (string_to_time_t(test_time_buffer, &parsed_time) == _SUCCESS) {  
            printf("Parsed time (time_t) from string: %ld\n", (long)parsed_time);  
			printf("parsed_time: %d\n",parsed_time);
        } else {  
            printf("Failed to parse time from string\n");  
        }  
    } else {  
        printf("Failed to convert time to string\n");  
    }  
  
    return 0;  
}


#endif






