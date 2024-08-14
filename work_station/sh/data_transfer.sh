#!/bin/bash
#check param
if [ $# -ne 2 ]; then
    echo "Usage:$0 <ifname> <param>"
    exit 1
fi


result=$(/bin/sh ./get_result.sh "$1" "$2")
echo $result

