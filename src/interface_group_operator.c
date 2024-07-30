
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
        while (1) {  // ʹ������ѭ����ȷ�����쳣�˳�ʱ���´����ӽ���  
            pid = fork();  
  
            if (pid == 0) {  
                // �ӽ���  
				while(1){
					test_upon_one_interface_in_one_time(get_interface_name(i), "hello, are you here?", PAKCAGES_NUM_ONE_TIME,choose_status_for_test); 
				}
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
                while(1){
                	listen_upon_one_interface_in_one_time(get_linked_node(i),get_interface_name(i),choose_status_for_listen); 
                }
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



#if 0
int main(int argc, char *argv[]) {
    if (argc != 6 && argc != 4) {  
        fprintf(stderr, "Usage: %s <config_file> <mode> <work_at_which_round_for_test_when_half_duplex> <work_at_which_round_for_listen_when_half_duplex> [ <center_interface_name> <res_file_name> (when listen mode)]\n", argv[0]);  
        fprintf(stderr, "Mode should be 'test' or 'listen'.\n");  
        return 1; // ��ʾ������Ϊ����Ĳ������˳�  
    }  
  
    const char* config_file = argv[1];  
    const char* mode = argv[2];
	work_at_which_round_for_test_when_half_duplex = argv[3];
	work_at_which_round_for_listen_when_half_duplex = argv[4];

	

    if (strcmp(mode, "test") == 0) {  
        start_and_load_info(config_file); 
		// ���濪ʼѭ�����Ը������úõ�����ͨ�Žӿ�
		test_upon_interface_group();
    } else if (strcmp(mode, "listen") == 0) {
        start_and_load_info(config_file);
		set_center_interface_name(argv[5]);
		set_res_file_name(argv[6]);
		listen_upon_interface_group();
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


#if 1


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



int main(int argc, char *argv[]) {  
	


  
    return 0;  
}


#endif




