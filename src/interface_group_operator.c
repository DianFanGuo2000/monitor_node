
#include "interface_group_operator.h"



/*

下面代码是对多进程中某些进程被异常中断后将其拉起来的功能的尝试，现在先挂起不做，如果后面有这个需要了再做

int *status_array;  
pthread_mutex_t *mutex_array;  
pid_t *pid_array;  
int array_size; // 假设您有一个表示数组大小的变量  
  
// Function to get the index of a PID in the pid_array  
int get_pid_index(pid_t *pid_array, pid_t pid, int size) {  
    for (int i = 0; i < size; i++) {  
        if (pid_array[i] == pid) {  
            return i;  
        }  
    }  
    return -1; // Return -1 if PID not found  
}  
  
void* child_handler(void* arg) {  
    pid_t pid = *(pid_t*)arg;  
    int i = get_pid_index(pid_array, pid, array_size);  
    if (i == -1) {  
        fprintf(stderr, "PID %d not found in pid_array\n", pid);  
        return NULL;  
    }  
  
    int status;  
    while (1) {  
        pid_t wpid = waitpid(pid, &status, WNOHANG);  
        if (wpid == pid) {  
            pthread_mutex_lock(&mutex_array[i]);  
            if (WIFEXITED(status)) {  
                printf("Child %d exited normally with status %d, restarting...\n", pid, WEXITSTATUS(status));  
                status_array[i] = WEXITSTATUS(status);  
            } else if (WIFSIGNALED(status)) {  
                printf("Child %d killed by signal %d, restarting...\n", pid, WTERMSIG(status));  
                status_array[i] = -WTERMSIG(status); // Use negative number to indicate killed by signal  
            }  
            pthread_mutex_unlock(&mutex_array[i]);  
            break; // Child process handled, exit loop  
        }  
        sleep(1); // Wait briefly before retrying  
    }  
    return NULL;  
}  



int *status_array;  
pthread_mutex_t *mutex_array;  
pid_t *pid_array;  
int array_size; // 假设您有一个表示数组大小的变量  

void test_upon_interface_group() {  
	int cnt = get_interface_cnt();  
    int i;  
    pid_t pid;  
    int status;
	status_array = malloc();
	status_array[i] = -1;
	pid_array = malloc();
	array_size = cnt;


  	while(1){
	    for (i = 0; i < cnt; i++) { 
			if status[i]==0{continue;}
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
				pthread_create(&thread, NULL, child_handler, &pid);  
	    }  
	}

    // 所有子进程（或它们的重启尝试）都已完成  
    while (wait(NULL) > 0);  
}  

*/



/*2024.8.5 不能使用多进程，因为底层的infor_manager是全局共享的，不同的进程会有自己独立的info_manager，破坏了这种共享，所以要改成多线程*/
/*
void test_upon_interface_group() {  
    int cnt = get_interface_cnt();  
    int i;  
    pid_t pid;  
    int status;  
  
    // Iterate through all interfaces  
    for (i = 0; i < cnt; i++) {  
        pid = fork();  
        if (pid == 0) {  
            // Child process  
            init_basic_interface(i);  
            while (1) {  
                test_upon_one_interface_in_one_time(get_interface_name_by_index(i), "hello, are you here?", PAKCAGES_NUM_ONE_TIME, choose_status_for_test);  
            }  
            exit(0); // Uncomment this line if you can exit the loop safely  
        } else if (pid < 0) {  
            // fork failed  
            perror("fork failed");  
            sleep(1);  // Brief wait before retrying  
            continue;  // Retry the fork  
        }  
    }  
  
    // Wait for all child processes to exit  
    // This loop will continue until there are no more child processes to wait for  
     while (wait(NULL) > 0);  
}  


void listen_upon_interface_group() {  
    int cnt = get_interface_cnt();  
    int i;  
    pid_t pid;  
    int status;  
  
    for (i = 0; i < cnt; i++) 
	{  
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
    }  

    // 所有子进程（或它们的重启尝试）都已完成  
    while (wait(NULL) > 0);  
}  

*/



// 线程函数  
void* test_interface_thread(void* arg) {  
    int index = *(int*)arg;  
    init_basic_interface(index);  
    while (1) {  
        test_upon_one_interface_in_one_time(get_interface_name_by_index(index), PAKCAGES_NUM_ONE_TIME);  
    }  
    // 注意：实际使用中，你可能需要一个机制来优雅地退出这个循环  
    return NULL;  
}  
  
void test_upon_interface_group() {  
    int cnt = get_interface_cnt();  
    pthread_t threads[cnt];  
    int indexes[cnt];  
    int i;  
  
    // 为每个接口创建一个线程  
    for (i = 0; i < cnt; i++) {  
        indexes[i] = i;  
        if (pthread_create(&threads[i], NULL, test_interface_thread, &indexes[i]) != 0) {  
            perror("pthread_create failed");  
            exit(EXIT_FAILURE);  
        }  
    }  
  
    // 等待所有线程完成  
    // 注意：由于我们的线程是无限循环的，这里实际上不会执行到这一行  
    // 除非你有其他机制来停止线程（如信号、条件变量等）  
    for (i = 0; i < cnt; i++) {  
        pthread_join(threads[i], NULL);  
    }  
  
    // 注意：在实际应用中，你可能不会这样等待无限循环的线程  
}  
  


// 线程函数  
void* listen_interface_thread(void* arg) {  
    int index = *(int*)arg;  
    init_basic_interface(index);  
    while (1) {  
        listen_upon_one_interface_in_one_time(get_linked_node(index),get_interface_name_by_index(index));  
    }  
    // 注意：实际使用中，你可能需要一个机制来优雅地退出这个循环  
    return NULL;  
}  
  
void listen_upon_interface_group() {  
    int cnt = get_interface_cnt();  
    pthread_t threads[cnt];  
    int indexes[cnt];  
    int i;  
  
    // 为每个接口创建一个线程  
    for (i = 0; i < cnt; i++) {  
        indexes[i] = i;  
        if (pthread_create(&threads[i], NULL, listen_interface_thread, &indexes[i]) != 0) {  
            perror("pthread_create failed");  
            exit(EXIT_FAILURE);  
        }  
    }  
  
    // 等待所有线程完成  
    // 注意：由于我们的线程是无限循环的，这里实际上不会执行到这一行  
    // 除非你有其他机制来停止线程（如信号、条件变量等）  
    for (i = 0; i < cnt; i++) {  
        pthread_join(threads[i], NULL);  
    }  
  
    // 注意：在实际应用中，你可能不会这样等待无限循环的线程  
}  
  


/*

// 下面函数虽然可以拉起死掉的子进程，但是只能执行第一个子进程，有很大的局限性，所以舍弃掉拉起死去子进程的功能

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

*/


#if 1


/*型式试验入口*/

#include <unistd.h> // 包含 access 函数的声明  

int is_valid_filename(const char *filename) {  
    // 首先检查指针是否为空和字符串是否为空  
    if (filename == NULL || *filename == '\0') {  
        // 这里的逻辑取决于你的定义：“空”或“空字符串”的 filename 是否被认为是“有效”的  
        // 在这个例子中，我们假设它们不是有效的  
        return 0;  
    }  
  
    // 使用 access 函数检查文件是否存在且可访问  
    // F_OK 检查文件是否存在  
    // 注意：这里的 R_OK, W_OK, X_OK 需要根据你的实际需求来选择或组合  
    // 但为了简单起见，这里只检查文件是否存在  
    if (access(filename, F_OK) == -1) {  
        // 如果 access 返回 -1，表示出现错误（文件不存在或没有权限等）  
        // 注意：在多线程或多进程环境中，文件可能在 access 调用之后立即被删除或修改  
        // 因此，这个检查并不是完全可靠的  
        // 检查 errno 可以提供更多关于为什么 access 失败的信息，但在这里我们简单返回 0  
        return 0;  
    }  
  
    // 如果 access 成功，我们假设文件名是有效的  
    return 1;  
}


int main(int argc, char *argv[]) {
	// 检查参数数量  
	if (argc < 3) {  
		// 参数不足  
		fprintf(stderr, "Usage: %s <config_file> test\n", argv[0]);  
		fprintf(stderr, "Or:	 %s <config_file> listen <res_file_name>\n", argv[0]);  
		fprintf(stderr, "Not enough arguments.\n");  
		return 1; // 表示程序因为错误的参数而退出  
	} else if (argc == 3) {  
		// 3个参数，检查模式是否为 'test'	
		if (strcmp(argv[2], "test") != 0) {  
			fprintf(stderr, "Invalid mode '%s'. Mode should be 'test' for 4 arguments.\n", argv[2]); 
			fprintf(stderr, "Usage: %s <config_file> test\n", argv[0]);  
			fprintf(stderr, "Or:	 %s <config_file> listen <res_file_name>\n", argv[0]); 

			return 1;  
		}  
		if (!is_valid_filename(argv[1])) {	
			fprintf(stderr, "Invalid round parameter '%s'. It should be a file path.\n", argv[1]);	
			fprintf(stderr, "Usage: %s <config_file> test\n", argv[0]);  
			fprintf(stderr, "Or:	 %s <config_file> listen <res_file_name>\n", argv[0]); 

			return 1;  
		}  

	} else if (argc == 4) {  
		// 4个参数，检查模式是否为 'listen'  
		if (strcmp(argv[2], "listen") != 0) {  
			fprintf(stderr, "Invalid mode '%s'. Mode should be 'listen' for 5 arguments.\n", argv[2]);	
			fprintf(stderr, "Usage: %s <config_file> test\n", argv[0]);  
			fprintf(stderr, "Or:	 %s <config_file> listen <res_file_name>\n", argv[0]); 

			return 1;  
		}  
		if (!is_valid_filename(argv[1])) {	
			fprintf(stderr, "Invalid round parameter '%s'. It should be a file path.\n", argv[1]);	
			fprintf(stderr, "Usage: %s <config_file> test\n", argv[0]);  
			fprintf(stderr, "Or:	 %s <config_file> listen <res_file_name>\n", argv[0]); 

			return 1;  
		}  

		if (!is_valid_filename(argv[3])) {	
			fprintf(stderr, "Invalid round parameter '%s'. It should be a file path.\n", argv[3]);	
			fprintf(stderr, "Usage: %s <config_file> test\n", argv[0]);  
			fprintf(stderr, "Or:	 %s <config_file> listen <res_file_name>\n", argv[0]);  
			return 1;  
		}  

	} else {  
		// 参数过多  
		fprintf(stderr, "Too many arguments.\n");  
		fprintf(stderr, "Usage: %s <config_file> test\n", argv[0]);  
		fprintf(stderr, "Or:	 %s <config_file> listen <res_file_name>\n", argv[0]); 

		return 1;  
	}  
	  

  

  
    // 如果是监听模式且参数数量不足  


  
    const char* config_file = argv[1];  
    const char* mode = argv[2];


    if (strcmp(mode, "test") == 0) {  
        start_and_load_info(config_file); 
		
		init_test_or_listen_record_arrays();
		
		// 下面开始循环测试各个配置好的物理通信接口
		test_upon_interface_group();
		
		free_test_or_listen_record_arrays();		
		
    } else if (strcmp(mode, "listen") == 0) {
        start_and_load_info(config_file);
		set_res_file_name(argv[3]);
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


#include <string.h> // 为了使用 memcpy  
#include <stdio.h>  // 为了使用 printf  
  
int main(int argc, char *argv[]) {  
    char *package_content = "HELLO CAN";  
    char send[16];  
    char recv[8]; // 初始化为0，确保是null结尾的字符串  
    int com1=1;
    int com2=4;

    memcpy(send, package_content, strlen(package_content)); // +1 是为了复制 null 结尾符  

    //udpCanStart();
  
    comCfgInit(com1, 1, 1000);  
    comCfgInit(com2, 1, 1000);  

    //sleep(1);
    //appCanDataRecv(com2, recv, sizeof(recv), -1); // 删除了多余的参数 
    //printf("recveived:"); 
    //for(int i=0;i<sizeof(recv);i++)
	//printf("%x ", recv[i]);  
    //printf("\n");  


    printf("sended data content: %s,com: %d\n", send,com1); 

    send_packet_can_fpu(com1,send, strlen(package_content)+1);
    //appCanDataSend(com1, 0x2ff, send, 8); // +1 是为了发送 null 结尾符 

    // 假设有一个合适的延迟函数，比如 TASK_DELAY();  
    //TASK_DELAY(); // 如果这个函数是必要的，请提供它的实现或声明  
    //usleep(200000);  


     

    receive_packet_can_fpu(com2, recv, 8, 2);
    printf("\nrecveived:"); 
    for(int i=0;i<sizeof(recv);i++)
	printf("%x ", recv[i]);  
    printf("\n");

    /*appCanDataRecv(com2, recv, 78, -1); // 删除了多余的参数  
    printf("recveived:"); 
    for(int i=0;i<sizeof(recv);i++)
	printf("%x ", recv[i]);  
    printf("\n");  


    appCanDataSendFunc(com1, send, 8); // +1 是为了发送 null 结尾符 
    usleep(200000);  

    appCanDataRecv(com2, recv, 78, -1); // 删除了多余的参数  
    printf("recveived:"); 
    for(int i=0;i<sizeof(recv);i++)
	printf("%x ", recv[i]);  
    printf("\n");*/

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



