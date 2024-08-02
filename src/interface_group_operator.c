
#include "interface_group_operator.h"




char* work_at_which_round_for_test_when_half_duplex;
char* work_at_which_round_for_listen_when_half_duplex;


// Function to choose the status for testing based on interface type and current round  
char* choose_status_for_test(const char *interface_name, const int current_round) {  
    char* interface_type = get_interface_type(interface_name); // Get the interface type  
    if (strcmp(interface_type, "eth") == 0) {  
        return "sending_and_receiving"; // For Ethernet, always send and receive  
    }  
    // Check if interface is CAN or RS485  
    if (strcmp(interface_type, "can") == 0 || strcmp(interface_type, "rs485") == 0) {  
        // Decide based on half-duplex configuration and current round  
        if (strcmp(work_at_which_round_for_test_when_half_duplex, "odd") == 0 && current_round % 2 == 1) {  
            return "sending"; // If odd rounds and configured for odd, send  
        }  
        if (strcmp(work_at_which_round_for_test_when_half_duplex, "even") == 0 && current_round % 2 == 0) {  
            return "sending"; // If even rounds and configured for even, send  
        }  
        return "receiving"; // If none of the above, receive  
    }  
    return "closed"; // Default status if interface type is not recognized  
}  
  
// Function to choose the status for listening based on interface type and current round  
char* choose_status_for_listen(const char *interface_name, const int current_round) {  
    char* interface_type = get_interface_type(interface_name); // Get the interface type  
    if (strcmp(interface_type, "eth") == 0) {  
        return "sending_and_receiving"; // For Ethernet, always send and receive  
    }  
    // Check if interface is CAN or RS485  
    if (strcmp(interface_type, "can") == 0 || strcmp(interface_type, "rs485") == 0) {  
        // Decide based on half-duplex configuration and current round  
        if (strcmp(work_at_which_round_for_listen_when_half_duplex, "odd") == 0 && current_round % 2 == 1) {  
            return "receiving"; // If odd rounds and configured for odd, receive  
        }  
        if (strcmp(work_at_which_round_for_listen_when_half_duplex, "even") == 0 && current_round % 2 == 0) {  
            return "receiving"; // If even rounds and configured for even, receive  
        }  
        return "sending"; // If none of the above, send (though typically in listen mode, it would be receiving)  
    }  
    return "closed"; // Default status if interface type is not recognized  
}




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
                init_basic_interface(i);
				while(1){
					test_upon_one_interface_in_one_time(get_interface_name_by_index(i), "hello, are you here?", PAKCAGES_NUM_ONE_TIME,choose_status_for_test); 
				}
				close_basic_interface(i);
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
                init_basic_interface(i);
                while(1){
                	listen_upon_one_interface_in_one_time(get_linked_node(i),get_interface_name_by_index(i),choose_status_for_listen); 
                }
				close_basic_interface(i);
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


/*型式试验入口*/



int main(int argc, char *argv[]) {
    if (argc != 5 && argc != 4) {  
        fprintf(stderr, "Usage: %s <config_file> <mode> <work_at_which_round_when_half_duplex> [<res_file_name> (when listen mode)]\n", argv[0]);  
        fprintf(stderr, "Mode should be 'test' or 'listen'.\n");  
        return 1; // 表示程序因为错误的参数而退出  
    }  
  
    const char* config_file = argv[1];  
    const char* mode = argv[2];

	
	if(strcmp(mode, "test") == 0)
	{
		work_at_which_round_for_test_when_half_duplex = argv[3];
	}

	if(strcmp(mode, "listen") == 0)
	{
		work_at_which_round_for_listen_when_half_duplex = argv[3];
	}

	

    if (strcmp(mode, "test") == 0) {  
        start_and_load_info(config_file); 
		
		init_test_or_listen_record_arrays();
		
		// 下面开始循环测试各个配置好的物理通信接口
		test_upon_interface_group();
		
		free_test_or_listen_record_arrays();		
		
    } else if (strcmp(mode, "listen") == 0) {
        start_and_load_info(config_file);
		set_res_file_name(argv[4]);
		init_test_or_listen_record_arrays();

		// 下面开始循环监听各个配置好的物理通信接口
		listen_upon_interface_group();

		free_test_or_listen_record_arrays();
    } else {  
        fprintf(stderr, "Invalid mode '%s'. Valid modes are 'test' 'listen'.\n", mode);  
        return 1; // 表示程序因为无效的模式而退出  
    }
		
    return 0; // 表示程序正常退出  
}

#endif




#if 0


/*时间转换测试*/


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


#if 0

/*之前的 CAN 测试代码*/



typedef int             INT32;
#define STATUS  int


extern INT32  udpCanStart(void);
extern STATUS canAndCanFdTest(void);



int main(int argc, char *argv[]) {  
	
	printf("x86 start \r\n");
	
		udpCanStart();
		
		canAndCanFdTest();
	
		
	while(1)
	{
		vos_EdrTaskDelay(5000);
	}
	
		return 1;

  
    return 0;  
}


#endif




#if 0

/*can自收自发测试*/


int main(int argc, char *argv[]) {  
	char res[10];
	comCanSTDCfgInit(1,500);
	comCanSTDCfgInit(2,500);
	send_packet_can(1,"hello",5);
	TASK_DELAY();
	receive_packet_can(2,res,5,1);

	printf("%s\n",res);
  
    return 0;  
}


#endif



#if 0

/*rs485发测试*/


int main(int argc, char *argv[]) {  
    int fd;
    int length;
    unsigned char msg[32] = {0};
    SerialPortParams params;
	params.baudrate = 115200;
	params.databits = 8;
	params.stopbits = 1;
	params.paritybits = 'N';

    fd = open_port("/dev/ttyS1", 0, params);

	if(fd < 0)
    {
        printf("Open port failed!\n");
        return _ERROR;
    }
    while(1)
	{
		char RS485MSG_ARRAY[RS485_LEN + 1];
		fillMessageToRS485Len("RS485 TEST!",RS485MSG_ARRAY,RS485_LEN);

        if (send_packet_rs485(fd, RS485MSG_ARRAY, RS485_LEN) < 0) {    
        	printf("send failed!\n");
        	return _ERROR; // Retry sending    
        }    
        printf("--------Write---------   %d: %s\n", RS485_LEN, RS485MSG_ARRAY);
		usleep(3000000);
        /*length = receive_packet_rs485(fd, msg, sizeof(RS485MSG), 5);
	        if(length > 0)
	        {	
	            printf("--------Read---------   %d: %s\n", length, msg);
	            if (strcmp(msg, RS485MSG)==0)
	            {
	                printf("ok\n");
	            }
	            usleep(3000000);
	        }
	        else
	        {
	            printf("TIME OUT.\n");
	            break;
	        }*/
    }
    printf("Close...\n");
    close_port(fd);
    return _SUCCESS; 
}


#endif




#if 0

/*rs485收测试*/


int main(int argc, char *argv[]) {
    int fd;
    int length;
    unsigned char msg[MAX_MSG_LEN+1] = {0};
    SerialPortParams params;
        params.baudrate = 115200;
        params.databits = 8;
        params.stopbits = 1;
        params.paritybits = 'N';

    fd = open_port("/dev/ttyS1", 0, params);

        if(fd < 0)
    {
        printf("Open port failed!\n");
        return _ERROR;
    }
    while(1)
        {
	printf("fd:%d RS485_LEN: %d\n", fd,MAX_MSG_LEN);
        //length = send_packet_rs485(fd, RS485MSG, sizeof(RS485MSG));
        //printf("--------Write---------   %d: %s\n", length, RS485MSG);
	//usleep(3000000);

        length = receive_packet_rs485(fd, msg, MAX_MSG_LEN, 5);
        if(length > 0)
        {
           printf("--------Read---------   %d: %s\n", length, msg);
		   printf("strlen(msg): %d\n", strlen(msg));
		   printf("strlen(RS485MSG): %d\n", strlen("RS485 TEST!"));
           if (strcmp(msg, "RS485 TEST!")==0)
          {
              printf("right\n");
           }
            usleep(3000000);
       }
       else
        {
            printf("TIME OUT.\n");
          //break;
       }
    }
    printf("Close...\n");
    close_port(fd);
    return _SUCCESS;
}


#endif



