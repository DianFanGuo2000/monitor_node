#!/bin/bash  
  
if [ $# -ne 1 ]; then  
    echo "Usage: $0 <search_target_json_path>"  
    exit 1  
fi  
  
# 获取第一个参数作为JSON文件的路径  
search_target_json_path=$1  
  
# 使用jq检查JSON是否为数组，并获取其长度  
cnt=$(jq 'if type == "array" then length else 0 end' "$search_target_json_path" 2>/dev/null)  
  
# 获取数组中第一个元素的updated_time字段，并将其转换为时间戳  
firsttime=$(jq -r ".[0].updated_time" "$search_target_json_path")  
base_tms=$(date -d "$firsttime" "+%s")  
  
# 遍历数组中的每个元素，检查updated_time字段  
i=1  
while [ $i -lt $cnt ]; do  
    time=$(jq -r ".[$i].updated_time" "$search_target_json_path")  
    tm_s=$(date -d "$time" "+%s")  
  
    # 如果找到更大的时间戳，则更新base_tms  
    if [ "$tm_s" -ge "$base_tms" ]; then  
        base_tms=$tm_s  
    fi  
    i=$((i + 1))  
done  
  
# 获取当前时间戳  
now_tms=$(date "+%s")  
  
# 计算时间差  
diff=$((now_tms - base_tms))  
  
# 如果时间差大于30秒，则输出0，否则输出1  
if [ $diff -gt 30 ]; then  
    echo 0  
else  
    echo 1  
fi
