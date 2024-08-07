#ifndef STATUS_CHOOSER_LIB_H  
#define STATUS_CHOOSER_LIB_H  


char *status_chooser_name_array[] = {"work_at_each_round", "only_work_at_odd_round", "only_work_at_even_round"};  
int status_chooser_num = 3;

char *status_chooser_transfer(char *status_chooser_name, const char *interface_name, const char *mode, const int current_round);
char* work_at_each_round(const char *interface_name, const char *mode, const int current_round);

char* only_work_at_odd_round(const char *interface_name, const char *mode, const int current_round);

char* only_work_at_even_round(const char *interface_name, const char *mode, const int current_round);



#endif

