#!/bin/sh  
      
# ��ȡ�ڶ���������ΪOID  
oid=$2  
      
# ����JSON�ļ���·��  
search_target_json_path=/home/work/monitor_nodeV8.9/work_station/res.json  
oid_configs_json_path=/home/work/monitor_nodeV8.9/work_station/snmp_work_station/oid_configs.json
      
# ʹ��jq�����й�����oid_configs�и���oid����if_name��if_property��if_id  
if_name=$(jq --raw-output ".[] | select(.oid == \"$oid\") | .if_name" $oid_configs_json_path)  
if_property=$(jq --raw-output ".[] | select(.oid == \"$oid\") | .if_property" $oid_configs_json_path)  
if_id=$(jq --raw-output ".[] | select(.oid == \"$oid\") | .if_id" $oid_configs_json_path)  

      
# ����if_property��ֵ�������  
if [ "$if_property" = "id" ]; then  
    echo $oid
    echo OctetString  
    echo $if_id  
elif [ "$if_property" = "if_active" ]; then  
    res=$(/bin/sh /home/work/monitor_nodeV8.9/work_station/snmp_work_station/snmpd_bash/bash_base/get_status.sh $search_target_json_path)   
    echo $oid 
    echo Integer32  
    echo $res  
else  
    res=$(/bin/sh /home/work/monitor_nodeV8.9/work_station/snmp_work_station/snmpd_bash/bash_base/get_result.sh $if_name $if_property $search_target_json_path)   
    echo $oid 
    echo Integer32  
    echo $res  
fi
