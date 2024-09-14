
#include "interface_group_operator.h"


/*
要做的:
1，(OK) 把通道同步个数多时序上加几个
2，加上对MCU通信的检查，一旦通信存在问题，就重启交换芯片，然后重启整个进程
3，加上对通信情况的检查，一旦有50%以上的端口出现问题，就直接杀掉当前所有线程所位于的进程，直接重启一个新的进程
4，(OK) 修改一下初始化的部分，把每个线程前初始化修改为启动线程前初始化
*/


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


/*
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

*/


/*
该应用可以进一步增加的功能点:
1，为了保证程序的健壮性，把整个程序放到子进程里面跑，死了就拉起来
2，对于复杂拓扑的配置，纯手动配置很耗时耗力，并且容易出错，可以搞一个图形化程序来拖拽配置拓扑，并且生成相应的配置文件
，这样又快又不会出错
*/



void test_upon_one_interface_in_one_test_case(const char *test_interface,int packages_num)
{
	while (1) {  
		test_upon_one_interface_in_one_time(test_interface, packages_num);
	}
}


typedef struct {  
    const char *linked_node;  
    const char *listened_interface;
} listen_thread_args;  



void *listen_thread_function(void *arg) {  
	pthread_detach(pthread_self());/*将当前线程与主线程分离，避免该线程结束后，线程栈和id并没有被释放*/
    listen_thread_args *ta = (listen_thread_args *)arg;  
	while(1)
	{
		listen_upon_one_interface_in_one_time(ta->linked_node, ta->listened_interface);
	}
    return NULL;  
}  



void listen_upon_one_interface_in_test_case(char *linked_node, char *listened_interface)
{
	pthread_t threads[LISTENING_THREAD_NUM];  
	listen_thread_args args[LISTENING_THREAD_NUM];  

	for (int i = 0; i < LISTENING_THREAD_NUM; i++) {  
	        args[i].linked_node = linked_node;  
	        args[i].listened_interface = listened_interface;  
			printf("Listen thread %d at listened interface \"%s\" for linked_node \"%s\" started!\n",i,listened_interface,linked_node);
	        pthread_create(&threads[i], NULL, listen_thread_function, &args[i]);  
	}  
	  
	for (int i = 0; i < LISTENING_THREAD_NUM; i++) {  
	    pthread_join(threads[i], NULL);  
	}  
}





// 线程函数  
void* interface_thread(void* arg) {
	pthread_detach(pthread_self());/*将当前线程与主线程分离，避免该线程结束后，线程栈和id并没有被释放*/
    int index = *(int*)arg;  
	
	if(strcmp(get_interface_mode_by_index(index),"test")==0)
	{
		test_upon_one_interface_in_one_test_case(get_interface_name_by_index(index), PAKCAGES_NUM_ONE_TIME);
	}
	if(strcmp(get_interface_mode_by_index(index),"listen")==0)
	{
		listen_upon_one_interface_in_test_case(get_linked_node(index),get_interface_name_by_index(index));
	} 

	closer_transfer(get_closer_name_by_index(index),get_interface_name_by_index(index));
    // 注意：实际使用中，你可能需要一个机制来优雅地退出这个循环  
    return NULL;  
}  

void test_or_listen_upon_interface_group() {  
    int cnt = get_interface_cnt();  
    pthread_t threads[cnt];  
    int indexes[cnt];  
	int initialized_flags[cnt]; 
    int i;  

	initialize_initializer_lock();
	for (i = 0; i < cnt; i++) 
	{  
	    int ret = initializer_transfer(get_initializer_name_by_index(i),get_interface_name_by_index(i)); 

		if(ret == _ERROR)
		{
			printf("[ERROR] cannot initialize the interface \"%s\"\n",get_interface_name_by_index(i));
			printf("[ERROR] refuse to start %s thread at the interface \"%s\"\n",get_interface_mode_by_index(i),get_interface_name_by_index(i));
			initialized_flags[i]=0; 
		}else
		{
			initialized_flags[i]=1; 
		}
	}

  
    // 为每个接口创建一个线程  
    for (i = 0; i < cnt; i++) { 
		if(initialized_flags[i]==0)
			continue;
        indexes[i] = i;  
        if (pthread_create(&threads[i], NULL, interface_thread, &indexes[i]) != 0) {  
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

  	destroy_initializer_lock();
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

#include <stdio.h>  
#include <string.h>  
#include <unistd.h>  
#include <errno.h>  
#include <stdio.h>  
#include <string.h>  
  
void input_split_and_copy(char *delimiter, int num_parts, char *dest[], size_t dest_size[]) {  
    char input[1024];  
    char *token;  
    int part_index = 0;  
  
    // 读取一行输入  
    if (fgets(input, sizeof(input), stdin) == NULL) {  
        return; // 读取失败或遇到文件结束符  
    }  
  
    // 移除换行符  
    input[strcspn(input, "\n")] = 0;  
  
    // 使用strtok分割字符串  
    token = strtok(input, delimiter);  
    while (token != NULL && part_index < num_parts) {  
        // 将分割出的部分复制到对应的目标数组中  
        strncpy(dest[part_index], token, dest_size[part_index] - 1);  
        dest[part_index][dest_size[part_index] - 1] = '\0'; // 确保字符串以null结尾  
        part_index++;  
  
        // 继续获取下一个分割出的部分  
        token = strtok(NULL, delimiter);  
    }  
  
    // 对于剩余的未填充的目标数组，设置为空字符串  
    for (; part_index < num_parts; part_index++) {  
        dest[part_index][0] = '\0';  
    }  
}  
  



int is_valid_filename(const char *filename) {  
    // 首先检查指针是否为空和字符串是否为空  
    if (filename == NULL || *filename == '\0') {  
        return 0;  
    }  

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

void createFileIfNotExists(const char* filePath) {  
    // Attempt to open the file in read-write mode ("r+")  
    FILE* file = fopen(filePath, "r+");  
  
    // If the file does not exist, fopen will return NULL  
    if (file == NULL) {  
        // Attempt to create the file in write mode ("w")  
        file = fopen(filePath, "w");  
        if (file == NULL) {  
            // If file creation fails, print an error message  
            perror("Failed to create file");  
            return;  
        }  
        printf("File created: %s\n", filePath);  
  
        // Close the file after creation  
        fclose(file);  
    } else {  
        // If the file already exists, close the file and print a message  
        printf("File already exists: %s\n", filePath);  
        fclose(file);  
    }  
}


int check_params_for_cmd_TLAtOneNodeFromSplitJsonFile(char *split_topology_config_file_name, char *res_file_name1) {  
    // 假设 is_valid_filename 是一个验证文件名有效性的函数  
    if (!is_valid_filename(split_topology_config_file_name)) {  
        fprintf(stderr, "Invalid parameter '%s', which is split topology config file path. It should be a valid file path.\n", split_topology_config_file_name);  
        return 0;  
    }  
    if (!is_valid_filename(res_file_name1)) {  
        fprintf(stderr, "Invalid parameter '%s', which is communication info save path. It should be a valid file path.\n", res_file_name1);  
        return 0;  
    }  
    return 1;  
}

int check_params_for_cmd_TLAtOneNodeFromOverallJsonFile(char *current_node_name, char *overall_topology_config_file_name , char *res_file_name1) {  
    // 假设 is_valid_filename 是一个验证文件名有效性的函数  
    if (!is_valid_filename(overall_topology_config_file_name)) {  
        fprintf(stderr, "Invalid parameter '%s', which is overall topology config file path. It should be a valid file path.\n", overall_topology_config_file_name);  
        return 0;  
    }  
    if (!is_valid_filename(res_file_name1)) {  
        fprintf(stderr, "Invalid parameter '%s', which is communication info save path. It should be a valid file path.\n", res_file_name1);  
        return 0;  
    }  
    return 1;  
}


int check_params_for_cmd_split_into_target(char *current_node_name, char *overall_topology_config_file_name , char *target_split_config_file_name) {  
    // 假设 is_valid_filename 是一个验证文件名有效性的函数  
    if (!is_valid_filename(overall_topology_config_file_name)) {  
        fprintf(stderr, "Invalid parameter '%s', which is overall topology config file path. It should be a valid file path.\n", overall_topology_config_file_name);  
        return 0;  
    }  
    if (!is_valid_filename(target_split_config_file_name)) {  
        fprintf(stderr, "Invalid parameter '%s', which is target split config file name. It should be a valid file path.\n", target_split_config_file_name);  
        return 0;  
    }  
    return 1;  
}



int check_params_for_cmd_xml_to_json(char *xml_config_path_node_if, char *xml_config_path_node_link, char *overall_topology_json_config_path) {  
    // 假设 is_valid_filename 是一个验证文件名有效性的函数  
    if (!is_valid_filename(xml_config_path_node_if)) {  
        fprintf(stderr, "Invalid parameter '%s', which is node if xml config path. It should be a valid file path.\n", xml_config_path_node_if);  
        return 0;  
    }  
	if (!is_valid_filename(xml_config_path_node_link)) {  
        fprintf(stderr, "Invalid parameter '%s', which is link xml config path. It should be a valid file path.\n", xml_config_path_node_link);  
        return 0;  
    }  
    if (!is_valid_filename(overall_topology_json_config_path)) {  
        fprintf(stderr, "Invalid parameter '%s', which is overall topology json config path. It should be a valid file path.\n", overall_topology_json_config_path);  
        return 0;  
    }  
    return 1;  
}


typedef void (*Func)();   
typedef int (*Restart_Check)();


// Function to execute a given function in a subprocess  
void execute_func_in_subproc(Func f,Restart_Check g) {  
    pid_t pid; // Use pid_t for portability  
	int reset_flag=0;
  	while(1)
	{
	    pid = fork(); // Create a new process  
	  
	    if (pid == 0) {  
	        // Child process  
	        f(); // Call the function passed as an argument  
	        exit(0); 
	    } else if (pid < 0) {  
	        // fork failed  
	        perror("fork failed");  
	        sleep(1); // Brief wait before retrying  
	    }  

	  	sleep(RESTART_CHECK_TIME_SPEC);
	    // Parent process: Wait for all child processes to exit  
	    // This loop will continue until there are no more child processes to wait for  
	    while (wait(NULL) > 0) {  
	        // wait(NULL) returns the PID of the child that exited, or -1 on error.  
	        // It returns 0 if there are no more child processes.  
	        // start the thread that is used to recurrently check the f execution status, if problem, then restart the f
			
			if(g()==0)
			{
				reset_flag=1;
				printf("as the restart situation is found, restart the whole proc now\n");
				break;
			}
			sleep(RESTART_CHECK_TIME_SPEC);
	    }  

		if(reset_flag==1)
		{	
			reset_flag=0;
			continue;
		}

  	}
}  
  

int restart_check() {  
    // 使用 -c 1 选项让 ping 只发送一个数据包，以便更快得到结果  
    int result = system("ping -c 1 192.168.1.199 > /dev/null 2>&1");  
  
    // 如果 system 调用失败（例如，命令不存在），则直接返回 0  
    if (result == -1) {  
        perror("ping 192.168.1.199 failed");  
        return 0;  
    }  

	
    return 1;  
}  


void TLAtOneNodeFromSplitJsonFile(char *split_config_file_name,char *res_file_name1)
{
    set_res_file_name(res_file_name1);


    start_info_manager_from_split_json(split_config_file_name); 
	
		
	init_test_or_listen_record_arrays();

	// 下面开始循环测试各个配置好的物理通信接口
	execute_func_in_subproc(test_or_listen_upon_interface_group,restart_check);


		
	free_test_or_listen_record_arrays();	

	close_info_manager(NULL);

}


void TLAtOneNodeFromOverallJsonFile(char *current_node_name,char *overall_config_file_name,char *res_file_name1)
{
	//printf("xsaasx\n");

    set_res_file_name(res_file_name1);


    start_info_manager_from_overall_json(current_node_name,overall_config_file_name); 
	
		
	init_test_or_listen_record_arrays();


		
	// 下面开始循环测试各个配置好的物理通信接口
	test_or_listen_upon_interface_group();

		
	free_test_or_listen_record_arrays();	

	close_info_manager(NULL);

}


void split_into_target(char *current_node_name,char *overall_config_file_name,char *target_split_config_file_name)
{
	createFileIfNotExists(target_split_config_file_name);
	start_info_manager_from_overall_json(current_node_name,overall_config_file_name); 
	close_info_manager(target_split_config_file_name);
}







int main(int argc, char *argv[]) { 

	char *parts[3];  
    size_t parts_size[3] = {50, 50, 50};  
    parts[0] = malloc(parts_size[0]);  
    parts[1] = malloc(parts_size[1]);  
    parts[2] = malloc(parts_size[2]);  
	char delimiter[5] = " ";


	if(argc==1)
	{
	    printf("welcome to communication test program...\n\n");  
	    printf("TLAtOneNodeFromSplitJsonFile(0)\t-- start to send, receive and record communication results at one node based on a split topology configure json file\n");  
		printf("TLAtOneNodeFromOverallJsonFile(1)\t-- start to send, receive and record communication results at one node based on a overall topology configure json file\n");  
		printf("split_into_target(2)\t-- split overall configure topology json file into target split topology configure json file at target node\n"); 
		printf("xml_to_json(3)\t-- convert xml configs to overall json config\n\n"); 


		
	  	
	    while (1) {  
	        printf("[cmd]: ");  
	        input_split_and_copy(delimiter,3, parts, parts_size);
	        if (strcmp(parts[0], "TLAtOneNodeFromSplitJsonFile") == 0 || strcmp(parts[0], "0") == 0) {  
	            printf("TLAtOneNodeFromSplitJsonFile Input reminder: <split_topology_config_file_name> <communication_info_file_save_path>\n"); 
				printf("[args for TLAtOneNodeFromSplitJsonFile]: ");
	            input_split_and_copy(delimiter,3, parts, parts_size);
	            while (check_params_for_cmd_TLAtOneNodeFromSplitJsonFile(parts[0], parts[1]) == 0)
	            {
	            	printf("[args for TLAtOneNodeFromSplitJsonFile]: ");
	            	input_split_and_copy(delimiter,3, parts, parts_size);
	            }
	            TLAtOneNodeFromSplitJsonFile(parts[0], parts[1]);  
	        } else if (strcmp(parts[0], "TLAtOneNodeFromOverallJsonFile") == 0 || strcmp(parts[0], "1") == 0) {  
	            printf("TLAtOneNodeFromOverallJsonFile Input reminder:<current_node_name> <overall_topology_config_file_name> <communication_info_file_save_path>\n"); 
				printf("[args for TLAtOneNodeFromOverallJsonFile]: ");
	            input_split_and_copy(delimiter,3, parts, parts_size);
	            while (check_params_for_cmd_TLAtOneNodeFromOverallJsonFile(parts[0], parts[1],parts[2]) == 0)
	            {
	            	printf("[args for TLAtOneNodeFromOverallJsonFile]: ");
	            	input_split_and_copy(delimiter,3, parts, parts_size);
	            }
	            TLAtOneNodeFromOverallJsonFile(parts[0], parts[1],parts[2]);  
	        } else if (strcmp(parts[0], "split_into_target") == 0 || strcmp(parts[0], "2") == 0) {  
				printf("split_into_target Input reminder:<current_node_name> <overall_topology_config_file_name> <target_split_config_file_save_path>\n"); 
				printf("[args for split_into_target]: ");
	            input_split_and_copy(delimiter,3, parts, parts_size);
			    while (check_params_for_cmd_split_into_target(parts[0], parts[1], parts[2]) == 0)
	            {
	            	printf("[args for split_into_target]: ");
	            	input_split_and_copy(delimiter,3, parts, parts_size);
	            }
				split_into_target(parts[0], parts[1],parts[2]);
	        } else if (strcmp(parts[0], "xml_to_json") == 0 || strcmp(parts[0], "3") == 0) {  
				printf("xml_to_json Input reminder: <xml_config_path_node_if> <xml_config_path_node_link> <overall_topology_json_config_path>\n"); 
				printf("[args for xml_to_json]: ");
	            input_split_and_copy(delimiter,3, parts, parts_size);
			    while (check_params_for_cmd_xml_to_json(parts[0], parts[1], parts[2]) == 0)
	            {
	            	printf("[args for xml_to_json]: ");
	            	input_split_and_copy(delimiter,3, parts, parts_size);
	            }
				convert_xml_config_to_overall_json_config(parts[0], parts[1], parts[2]);
	        } 
	    }
	}else
	{
		if(strcmp(argv[1], "0") == 0)
		{
			if(check_params_for_cmd_TLAtOneNodeFromSplitJsonFile(argv[2], argv[3]) == 0)
			{
				return 0;
			}

			TLAtOneNodeFromSplitJsonFile(argv[2], argv[3]);
		}
		if(strcmp(argv[1], "1") == 0)
		{
			if(check_params_for_cmd_TLAtOneNodeFromOverallJsonFile(argv[2], argv[3],argv[4]) == 0)
			{
				return 0;
			}
			TLAtOneNodeFromOverallJsonFile(argv[2], argv[3],argv[4]);
		}
		if(strcmp(argv[1], "2") == 0)
		{
			if(check_params_for_cmd_split_into_target(argv[2], argv[3],argv[4]) == 0)
			{
				return 0;
			}
			split_into_target(argv[2], argv[3],argv[4]);
		}
		if(strcmp(argv[1], "3") == 0)
		{
			if(check_params_for_cmd_xml_to_json(argv[2], argv[3],argv[4]) == 0)
			{
				return 0;
			}
			convert_xml_config_to_overall_json_config(argv[2], argv[3],argv[4]);
		}
	}
		

  

    free(parts[0]);
	free(parts[1]);
	free(parts[2]);  

    return 0;  
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



