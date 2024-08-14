#!/bin/sh
oid=$2
cd /home/zc/monitor_node-main/work_station

if [ $oid = ".1.3.6.1.4.1.37720.4.5.2.1.1" ]; then
	id=$(/bin/sh ./data_transfer.sh eth1 id)
	echo $oid
	echo Integer32
	echo $id
elif [ $oid = ".1.3.6.1.4.1.37720.4.5.2.1.2" ]; then 
	rx=$(/bin/sh ./data_transfer.sh eth1 rx)
	echo $oid
	echo Integer32
	echo $rx
elif [ $oid = ".1.3.6.1.4.1.37720.4.5.2.1.3" ]; then 
	tx=$(/bin/sh ./data_transfer.sh eth1 tx)
	echo $oid
	echo Integer32
	echo $tx
elif [ $oid = ".1.3.6.1.4.1.37720.4.5.2.2.1" ]; then
	id=$(/bin/sh ./data_transfer.sh eth2 id)
	echo $oid
	echo Integer32
	echo $id
elif [ $oid = ".1.3.6.1.4.1.37720.4.5.2.2.2" ]; then 
	rx=$(/bin/sh ./data_transfer.sh eth2 rx)
	echo $oid
	echo Integer32
	echo $rx
elif [ $oid = ".1.3.6.1.4.1.37720.4.5.2.2.3" ]; then 
	tx=$(/bin/sh ./data_transfer.sh eth2 tx)
	echo $oid
	echo Integer32
	echo $tx
elif [ $oid = ".1.3.6.1.4.1.37720.4.5.2.3.1" ]; then
	id=$(/bin/sh ./data_transfer.sh eth3 id)
	echo $oid
	echo Integer32
	echo $id
elif [ $oid = ".1.3.6.1.4.1.37720.4.5.2.3.2" ]; then 
	rx=$(/bin/sh ./data_transfer.sh eth3 rx)
	echo $oid
	echo Integer32
	echo $rx
elif [ $oid = ".1.3.6.1.4.1.37720.4.5.2.3.3" ]; then 
	tx=$(/bin/sh ./data_transfer.sh eth3 tx)
	echo $oid
	echo Integer32
	echo $tx
elif [ $oid = ".1.3.6.1.4.1.37720.4.4.2.1.1" ]; then
	id=$(/bin/sh ./data_transfer.sh can0 id)
	echo $oid
	echo Integer32
	echo $id
elif [ $oid = ".1.3.6.1.4.1.37720.4.4.2.1.2" ]; then 
	rx=$(/bin/sh ./data_transfer.sh can0 rx)
	echo $oid
	echo Integer32
	echo $rx
elif [ $oid = ".1.3.6.1.4.1.37720.4.4.2.1.3" ]; then 
	tx=$(/bin/sh ./data_transfer.sh can0 tx)
	echo $oid
	echo Integer32
	echo $tx
elif [ $oid = ".1.3.6.1.4.1.37720.4.4.2.2.1" ]; then
	id=$(/bin/sh ./data_transfer.sh can1 id)
	echo $oid
	echo Integer32
	echo $id
elif [ $oid = ".1.3.6.1.4.1.37720.4.4.2.2.2" ]; then 
	rx=$(/bin/sh ./data_transfer.sh can1 rx)
	echo $oid
	echo Integer32
	echo $rx
elif [ $oid = ".1.3.6.1.4.1.37720.4.4.2.2.3" ]; then 
	tx=$(/bin/sh ./data_transfer.sh can1 tx)
	echo $oid
	echo Integer32
	echo $tx
elif [ $oid = ".1.3.6.1.4.1.37720.4.12.2.1.2" ]; then 
	rx=$(/bin/sh ./data_transfer.sh 485 rx)
	echo $oid
	echo Integer32
	echo $rx
elif [ $oid = ".1.3.6.1.4.1.37720.4.12.2.1.3" ]; then 
	tx=$(/bin/sh ./data_transfer.sh 485 tx)
	echo $oid
	echo Integer32
	echo $tx
fi

	

