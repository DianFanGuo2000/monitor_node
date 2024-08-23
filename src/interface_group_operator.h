#ifndef INTERFACE_GROUP_OPERATOR_H  
#define INTERFACE_GROUP_OPERATOR_H  


#include "one_interface_operator.h"


/*
void test_upon_interface_group();
void listen_upon_interface_group();
*/

void test_or_listen_upon_interface_group(); 

int check_params_for_cmd_TLAtOneNodeFromSplitJsonFile(char *split_topology_config_file_name, char *res_file_name1);
int check_params_for_cmd_TLAtOneNodeFromOverallJsonFile(char *current_node_name, char *overall_topology_config_file_name , char *res_file_name1);

int check_params_for_cmd_split_into_target(char *current_node_name, char *overall_topology_config_file_name , char *target_split_config_file_name);


void TLAtOneNodeFromSplitJsonFile(char *split_config_file_name,char *res_file_name1);

void TLAtOneNodeFromOverallJsonFile(char *current_node_name,char *overall_config_file_name,char *res_file_name1);

void split_into_target(char *current_node_name,char *overall_config_file_name,char *target_split_config_file_name);

#endif
