#!/bin/sh
if [ $# -ne 3 ]; then
    echo "Usage:$0 <ifname> <param> <search_target_json_path>"
    exit 1
fi

ifname=$1
param=$2
search_target_json_path=$3


uptime_t=$(jq --arg ifname "$ifname" -r '.[] | select(.interface_name == $ifname).updated_time' "$search_target_json_path")
uptime=$(date -d "$uptime_t" "+%s")
nowtime=$(date "+%s")
diff=$((nowtime - uptime))

if [ $diff -gt 30 ]; then 
    echo 0
    return
fi


if [ "$param" = "rx" ]; then 
    rx=$(jq --arg ifname "$ifname" -r '.[] | select(.interface_name == $ifname).rx' "$search_target_json_path")
    if [ $rx -gt 15 ]; then 
        re=1
    else
        re=0
    fi
    echo $re
elif [ "$param" = "tx" ]; then
    tx=$(jq --arg ifname "$ifname" -r '.[] | select(.interface_name == $ifname).tx' "$search_target_json_path")
    echo $tx
else
    echo "error param '$param'"
fi

