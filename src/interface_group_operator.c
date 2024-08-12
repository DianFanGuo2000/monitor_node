
#include "interface_group_operator.h"



/*

��������ǶԶ������ĳЩ���̱��쳣�жϺ����������Ĺ��ܵĳ��ԣ������ȹ���������������������Ҫ������

int *status_array;  
pthread_mutex_t *mutex_array;  
pid_t *pid_array;  
int array_size; // ��������һ����ʾ�����С�ı���  
  
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
int array_size; // ��������һ����ʾ�����С�ı���  

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
	                // �ӽ���  
	                init_basic_interface(i);
					while(1){
						test_upon_one_interface_in_one_time(get_interface_name_by_index(i), "hello, are you here?", PAKCAGES_NUM_ONE_TIME,choose_status_for_test); 
					}
					close_basic_interface(i);
	                exit(0); // �ӽ�����ɺ��˳�  
	            } else if (pid < 0) {  
	                // fork ʧ��  
	                perror("fork failed");  
	                sleep(1);  // ���ݵȴ�������  
	                continue;  // �ص�ѭ����ʼ�����³���fork  
	            } 
				pthread_create(&thread, NULL, child_handler, &pid);  
	    }  
	}

    // �����ӽ��̣������ǵ��������ԣ��������  
    while (wait(NULL) > 0);  
}  

*/



/*2024.8.5 ����ʹ�ö���̣���Ϊ�ײ��infor_manager��ȫ�ֹ���ģ���ͬ�Ľ��̻����Լ�������info_manager���ƻ������ֹ�������Ҫ�ĳɶ��߳�*/
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
                // �ӽ���  
                init_basic_interface(i);
                while(1){
                	listen_upon_one_interface_in_one_time(get_linked_node(i),get_interface_name_by_index(i),choose_status_for_listen); 
                }
				close_basic_interface(i);
                exit(0); // �ӽ�����ɺ��˳�  
            } else if (pid < 0) {  
                // fork ʧ��  
                perror("fork failed");  
                sleep(1);  // ���ݵȴ�������  
                continue;  // �ص�ѭ����ʼ�����³���fork  
            }  
    }  

    // �����ӽ��̣������ǵ��������ԣ��������  
    while (wait(NULL) > 0);  
}  

*/



// �̺߳���  
void* test_interface_thread(void* arg) {  
    int index = *(int*)arg;  
    init_basic_interface(index);  
    while (1) {  
        test_upon_one_interface_in_one_time(get_interface_name_by_index(index), PAKCAGES_NUM_ONE_TIME);  
    }  
    // ע�⣺ʵ��ʹ���У��������Ҫһ�����������ŵ��˳����ѭ��  
    return NULL;  
}  
  
void test_upon_interface_group() {  
    int cnt = get_interface_cnt();  
    pthread_t threads[cnt];  
    int indexes[cnt];  
    int i;  
  
    // Ϊÿ���ӿڴ���һ���߳�  
    for (i = 0; i < cnt; i++) {  
        indexes[i] = i;  
        if (pthread_create(&threads[i], NULL, test_interface_thread, &indexes[i]) != 0) {  
            perror("pthread_create failed");  
            exit(EXIT_FAILURE);  
        }  
    }  
  
    // �ȴ������߳����  
    // ע�⣺�������ǵ��߳�������ѭ���ģ�����ʵ���ϲ���ִ�е���һ��  
    // ������������������ֹͣ�̣߳����źš����������ȣ�  
    for (i = 0; i < cnt; i++) {  
        pthread_join(threads[i], NULL);  
    }  
  
    // ע�⣺��ʵ��Ӧ���У�����ܲ��������ȴ�����ѭ�����߳�  
}  
  


// �̺߳���  
void* listen_interface_thread(void* arg) {  
    int index = *(int*)arg;  
    init_basic_interface(index);  
    while (1) {  
        listen_upon_one_interface_in_one_time(get_linked_node(index),get_interface_name_by_index(index));  
    }  
    // ע�⣺ʵ��ʹ���У��������Ҫһ�����������ŵ��˳����ѭ��  
    return NULL;  
}  
  
void listen_upon_interface_group() {  
    int cnt = get_interface_cnt();  
    pthread_t threads[cnt];  
    int indexes[cnt];  
    int i;  
  
    // Ϊÿ���ӿڴ���һ���߳�  
    for (i = 0; i < cnt; i++) {  
        indexes[i] = i;  
        if (pthread_create(&threads[i], NULL, listen_interface_thread, &indexes[i]) != 0) {  
            perror("pthread_create failed");  
            exit(EXIT_FAILURE);  
        }  
    }  
  
    // �ȴ������߳����  
    // ע�⣺�������ǵ��߳�������ѭ���ģ�����ʵ���ϲ���ִ�е���һ��  
    // ������������������ֹͣ�̣߳����źš����������ȣ�  
    for (i = 0; i < cnt; i++) {  
        pthread_join(threads[i], NULL);  
    }  
  
    // ע�⣺��ʵ��Ӧ���У�����ܲ��������ȴ�����ѭ�����߳�  
}  
  


/*

// ���溯����Ȼ���������������ӽ��̣�����ֻ��ִ�е�һ���ӽ��̣��кܴ�ľ����ԣ�����������������ȥ�ӽ��̵Ĺ���

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
                init_basic_interface(i);
				while(1){
					test_upon_one_interface_in_one_time(get_interface_name_by_index(i), "hello, are you here?", PAKCAGES_NUM_ONE_TIME,choose_status_for_test); 
				}
				close_basic_interface(i);
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
                init_basic_interface(i);
                while(1){
                	listen_upon_one_interface_in_one_time(get_linked_node(i),get_interface_name_by_index(i),choose_status_for_listen); 
                }
				close_basic_interface(i);
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

*/


#if 1


/*��ʽ�������*/

#include <unistd.h> // ���� access ����������  

int is_valid_filename(const char *filename) {  
    // ���ȼ��ָ���Ƿ�Ϊ�պ��ַ����Ƿ�Ϊ��  
    if (filename == NULL || *filename == '\0') {  
        // ������߼�ȡ������Ķ��壺���ա��򡰿��ַ������� filename �Ƿ���Ϊ�ǡ���Ч����  
        // ����������У����Ǽ������ǲ�����Ч��  
        return 0;  
    }  
  
    // ʹ�� access ��������ļ��Ƿ�����ҿɷ���  
    // F_OK ����ļ��Ƿ����  
    // ע�⣺����� R_OK, W_OK, X_OK ��Ҫ�������ʵ��������ѡ������  
    // ��Ϊ�˼����������ֻ����ļ��Ƿ����  
    if (access(filename, F_OK) == -1) {  
        // ��� access ���� -1����ʾ���ִ����ļ������ڻ�û��Ȩ�޵ȣ�  
        // ע�⣺�ڶ��̻߳����̻����У��ļ������� access ����֮��������ɾ�����޸�  
        // ��ˣ������鲢������ȫ�ɿ���  
        // ��� errno �����ṩ�������Ϊʲô access ʧ�ܵ���Ϣ�������������Ǽ򵥷��� 0  
        return 0;  
    }  
  
    // ��� access �ɹ������Ǽ����ļ�������Ч��  
    return 1;  
}


int main(int argc, char *argv[]) {
	// ����������  
	if (argc < 3) {  
		// ��������  
		fprintf(stderr, "Usage: %s <config_file> test\n", argv[0]);  
		fprintf(stderr, "Or:	 %s <config_file> listen <res_file_name>\n", argv[0]);  
		fprintf(stderr, "Not enough arguments.\n");  
		return 1; // ��ʾ������Ϊ����Ĳ������˳�  
	} else if (argc == 3) {  
		// 3�����������ģʽ�Ƿ�Ϊ 'test'	
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
		// 4�����������ģʽ�Ƿ�Ϊ 'listen'  
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
		// ��������  
		fprintf(stderr, "Too many arguments.\n");  
		fprintf(stderr, "Usage: %s <config_file> test\n", argv[0]);  
		fprintf(stderr, "Or:	 %s <config_file> listen <res_file_name>\n", argv[0]); 

		return 1;  
	}  
	  

  

  
    // ����Ǽ���ģʽ�Ҳ�����������  


  
    const char* config_file = argv[1];  
    const char* mode = argv[2];


    if (strcmp(mode, "test") == 0) {  
        start_and_load_info(config_file); 
		
		init_test_or_listen_record_arrays();
		
		// ���濪ʼѭ�����Ը������úõ�����ͨ�Žӿ�
		test_upon_interface_group();
		
		free_test_or_listen_record_arrays();		
		
    } else if (strcmp(mode, "listen") == 0) {
        start_and_load_info(config_file);
		set_res_file_name(argv[3]);
		init_test_or_listen_record_arrays();

		// ���濪ʼѭ�������������úõ�����ͨ�Žӿ�
		listen_upon_interface_group();

		free_test_or_listen_record_arrays();
    } else {  
        fprintf(stderr, "Invalid mode '%s'. Valid modes are 'test' 'listen'.\n", mode);  
        return 1; // ��ʾ������Ϊ��Ч��ģʽ���˳�  
    }
		
    return 0; // ��ʾ���������˳�  
}

#endif




#if 0


/*ʱ��ת������*/


int main(int argc, char *argv[]) {  
    time_t parsed_time = time(NULL);  // ��ȡ��ǰʱ��  
    char time_str[80];  
  
    // �� time_t ת��Ϊ�ַ���  
    if (time_t_to_string(parsed_time, time_str, sizeof(time_str)) == _SUCCESS) {  
        printf("Converted time to string: %s\n", time_str);  
  
        // ʹ��һ����֪��Ч��ʱ���ַ��������� string_to_time_t  
        const char* test_time_buffer = "Tue Mar  3 10:03:58 2020"; // ʾ��ʱ���ַ���  
  
        // ���ַ���ת���� time_t  
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

/*֮ǰ�� CAN ���Դ���*/



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

/*can�����Է�����*/


#include <string.h> // Ϊ��ʹ�� memcpy  
#include <stdio.h>  // Ϊ��ʹ�� printf  
  
int main(int argc, char *argv[]) {  
    char *package_content = "HELLO CAN";  
    char send[16];  
    char recv[8]; // ��ʼ��Ϊ0��ȷ����null��β���ַ���  
    int com1=1;
    int com2=4;

    memcpy(send, package_content, strlen(package_content)); // +1 ��Ϊ�˸��� null ��β��  

    //udpCanStart();
  
    comCfgInit(com1, 1, 1000);  
    comCfgInit(com2, 1, 1000);  

    //sleep(1);
    //appCanDataRecv(com2, recv, sizeof(recv), -1); // ɾ���˶���Ĳ��� 
    //printf("recveived:"); 
    //for(int i=0;i<sizeof(recv);i++)
	//printf("%x ", recv[i]);  
    //printf("\n");  


    printf("sended data content: %s,com: %d\n", send,com1); 

    send_packet_can_fpu(com1,send, strlen(package_content)+1);
    //appCanDataSend(com1, 0x2ff, send, 8); // +1 ��Ϊ�˷��� null ��β�� 

    // ������һ�����ʵ��ӳٺ��������� TASK_DELAY();  
    //TASK_DELAY(); // �����������Ǳ�Ҫ�ģ����ṩ����ʵ�ֻ�����  
    //usleep(200000);  


     

    receive_packet_can_fpu(com2, recv, 8, 2);
    printf("\nrecveived:"); 
    for(int i=0;i<sizeof(recv);i++)
	printf("%x ", recv[i]);  
    printf("\n");

    /*appCanDataRecv(com2, recv, 78, -1); // ɾ���˶���Ĳ���  
    printf("recveived:"); 
    for(int i=0;i<sizeof(recv);i++)
	printf("%x ", recv[i]);  
    printf("\n");  


    appCanDataSendFunc(com1, send, 8); // +1 ��Ϊ�˷��� null ��β�� 
    usleep(200000);  

    appCanDataRecv(com2, recv, 78, -1); // ɾ���˶���Ĳ���  
    printf("recveived:"); 
    for(int i=0;i<sizeof(recv);i++)
	printf("%x ", recv[i]);  
    printf("\n");*/

    return 0;  
}

#endif



#if 0

/*rs485������*/


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

/*rs485�ղ���*/


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



