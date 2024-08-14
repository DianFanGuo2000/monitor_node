#!/bin/bash
if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <interface_name>"
    exit 1
fi

name=$1
id=${name:3:1}
echo $id
