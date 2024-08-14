#!/bin/bash
if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <interface_name>"
    exit 1
fi

name=$1
uptime=$(jq --arg name $name -r '.[] | select(.interface_name == $name) | [.updated_time] | @tsv' res.json)
tm_s=$(date -d "$uptime" "+%s")
echo $tm_s


