#!/bin/bash  
  
if [ $# -ne 1 ]; then  
    echo "Usage: $0 <search_target_json_path>"  
    exit 1  
fi  
  
# ��ȡ��һ��������ΪJSON�ļ���·��  
search_target_json_path=$1  
  
# ʹ��jq���JSON�Ƿ�Ϊ���飬����ȡ�䳤��  
cnt=$(jq 'if type == "array" then length else 0 end' "$search_target_json_path" 2>/dev/null)  
  
# ��ȡ�����е�һ��Ԫ�ص�updated_time�ֶΣ�������ת��Ϊʱ���  
firsttime=$(jq -r ".[0].updated_time" "$search_target_json_path")  
base_tms=$(date -d "$firsttime" "+%s")  
  
# ���������е�ÿ��Ԫ�أ����updated_time�ֶ�  
i=1  
while [ $i -lt $cnt ]; do  
    time=$(jq -r ".[$i].updated_time" "$search_target_json_path")  
    tm_s=$(date -d "$time" "+%s")  
  
    # ����ҵ������ʱ����������base_tms  
    if [ "$tm_s" -ge "$base_tms" ]; then  
        base_tms=$tm_s  
    fi  
    i=$((i + 1))  
done  
  
# ��ȡ��ǰʱ���  
now_tms=$(date "+%s")  
  
# ����ʱ���  
diff=$((now_tms - base_tms))  
  
# ���ʱ������30�룬�����0���������1  
if [ $diff -gt 30 ]; then  
    echo 0  
else  
    echo 1  
fi
