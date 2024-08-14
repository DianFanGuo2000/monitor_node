#!/bin/bash
if [ $# -ne 2 ]; then
    echo "Usage:$0 <ifname> <param>"
    exit 1
fi

ifname=$1
param=$2

uptime=$(/bin/sh ./get_uptime.sh "$ifname")
nowtime=$(date "+%s")
diff=$((nowtime - uptime))

if [ $diff -lt 10 ]; then 
    #Timeout
    return 1
fi

if [ "$param" = "id" ]; then 
    id=$(/bin/bash ./get_id.sh "$ifname")
    echo $id
elif [ "$param" = "rx" ]; then 
    rx=$(/bin/bash ./get_rx.sh "$ifname")
    echo $rx
elif [ "$param" = "tx" ]; then
    tx=$(/bin/bash ./get_tx.sh "$ifname")
    echo $tx
else
    echo "error param '$param'"
fi

