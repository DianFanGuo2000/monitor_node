#!/bin/bash  
  
snmpd_agent_bash_path=$1  
get_status_bash_path=$2 
get_result_bash_path=$3
oid_configs_json_path=$4 
ori_snmpd_conf_path=$5   
snmpd_conf_path=$6  
search_target_json_path=$7 





# 为snmpd_agent_bash_path指定的脚本设置内容  
printf '%s\n' "#!/bin/sh  
      
# 获取第二个参数作为OID  
oid=\$2  
      
# 定义JSON文件的路径  
search_target_json_path=$search_target_json_path  
oid_configs_json_path=$oid_configs_json_path
      
# 使用jq命令行工具在oid_configs中根据oid查找if_name、if_property和if_id  
if_name=\$(jq --raw-output \".[] | select(.oid == \\\"\$oid\\\") | .if_name\" \$oid_configs_json_path)  
if_property=\$(jq --raw-output \".[] | select(.oid == \\\"\$oid\\\") | .if_property\" \$oid_configs_json_path)  
if_id=\$(jq --raw-output \".[] | select(.oid == \\\"\$oid\\\") | .if_id\" \$oid_configs_json_path)  

      
# 根据if_property的值决定输出  
if [ \"\$if_property\" = \"id\" ]; then  
    echo \$oid
    echo OctetString  
    echo \$if_id  
elif [ \"\$if_property\" = \"if_active\" ]; then  
    res=\$(/bin/sh $get_status_bash_path \$search_target_json_path)   
    echo \$oid 
    echo Integer32  
    echo \$res  
else  
    res=\$(/bin/sh $get_result_bash_path \$if_name \$if_property \$search_target_json_path)   
    echo \$oid 
    echo Integer32  
    echo \$res  
fi" > "$snmpd_agent_bash_path"
  
# 使脚本可执行  
sudo chmod +x "$snmpd_agent_bash_path"  
sudo chmod +x "$get_status_bash_path"  
sudo chmod +x "$get_result_bash_path"  


sudo cp $ori_snmpd_conf_path $snmpd_conf_path
  
  
# 初始化str_t为空  
str_t=""  
  
# 遍历JSON文件，提取oid，并构建pass语句  
oid_list=$(jq -r '.[].oid' "$oid_configs_json_path")  
for oid in $oid_list; do  
    new_pass="pass $oid /bin/sh $snmpd_agent_bash_path"  
    # 检查新构建的pass语句是否已存在于snmpd.conf文件中  
    if ! grep -qFx "$new_pass" "$snmpd_conf_path"; then  
        str_t+="$new_pass\n"  
    fi  
done  
  
# 如果str_t不为空，则将其插入到snmpd.conf文件的指定行之后  
if [ -n "$str_t" ]; then  
    awk -v str="$str_t" '  
    /pass \.1\.3\.6\.1\.4\.1\.8072\.2\.255  \/usr\/bin\/perl PREFIX\/local\/passtest\.pl/{  
        print $0;  
        print str;  
        next;  
    }  
    { print }  
    ' "$snmpd_conf_path" > "${snmpd_conf_path}.tmp" && mv "${snmpd_conf_path}.tmp" "$snmpd_conf_path"  
  
    # 检查更新是否成功  
    if [ $? -eq 0 ]; then  
        echo "snmpd.conf updated successfully..."  
        systemctl restart snmpd  
        echo "snmpd restarted"  
    else  
        echo "failed to update snmpd.conf"  
    fi  
else  
    echo "No new pass statements to add to snmpd.conf"  
fi
