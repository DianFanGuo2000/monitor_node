#!/bin/bash
if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <interface_name>"
    exit 1
fi

name=$1
rx=$(jq --arg name $name -r '.[] | select(.interface_name == $name) | [.rx] | @tsv' res.json)
echo $rx
