#!/bin/bash  
  
# 确保传入了 IP 地址  
if [ -z "$1" ]; then  
    echo "Usage: $0 <NTP-server-IP>"  
    exit 1  
fi  
  
# 将配置写入 timesyncd.conf  
echo "  
#  This file is part of systemd.  
#  
#  systemd is free software; you can redistribute it and/or modify it  
#  under the terms of the GNU Lesser General Public License as published by  
#  the Free Software Foundation; either version 2.1 of the License, or  
#  (at your option) any later version.  
#  
# Entries in this file show the compile time defaults.  
# You can change settings by editing this file.  
# Defaults can be restored by simply deleting this file.  
#  
# See timesyncd.conf(5) for details.  
  
[Time]  
NTP=$1  
#FallbackNTP=ntp.ubuntu.com  
#RootDistanceMaxSec=5  
#PollIntervalMinSec=32  
#PollIntervalMaxSec=2048  
" >> /etc/systemd/timesyncd.conf  
  
# 设置系统时间和时区  
sudo timedatectl set-local-rtc 0  
sudo timedatectl set-ntp on  
sudo timedatectl set-timezone Asia/Shanghai  
sudo systemctl restart systemd-timesyncd  
timedatectl status