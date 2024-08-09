
#include "can_recv_send.h"



int get_cpu_freq_mhz_linux() {  
    char buffer[1024];  
    FILE *fp = popen("cat /proc/cpuinfo | grep 'cpu MHz' | head -n 1", "r");  
    if (fp == NULL) {  
        printf("Failed to run command\n");  
        exit(1);  
    }  
  
    if (fgets(buffer, sizeof(buffer), fp) != NULL) {  
        // 假设格式总是 "cpu MHz : XXXX.XXX\n"，并解析出 XXXX 部分  
        char *end;  
        long freq_mhz = strtol(strchr(buffer, ':') + 2, &end, 10);  
        if (end != buffer + strlen(buffer) - 1 && *end == '\n') {  
            // 如果解析成功并且字符串以换行符结尾，则返回频率  
            pclose(fp);  
            return (int)freq_mhz;  
        }  
    }  
  
    pclose(fp);  
    return -1; // 如果解析失败，则返回 -1  
}  


int receive_packet_can_fpu(UINT32 can_channel_id, char *msg, UINT32 length, int wait_time)
{
	int ticks_spec = get_cpu_freq_mhz_linux()* 1000000 * wait_time;
	int ret = appCanDataRecv(can_channel_id,msg,length,ticks_spec); // 注意，这里的时间间隔只有在vxworks系统中才会生效
	if(ret<0)
		return _ERROR;
	return _SUCCESS;
}

int send_packet_can_fpu(UINT32 can_channel_id, const char *msg, UINT32 length)
{
	int ret = appCanDataSendFunc(can_channel_id,msg,length);
	if(ret<0)
		return _ERROR;
	return _SUCCESS;
}





