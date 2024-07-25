#!/bin/bash  
  
# 定义一个函数来设置系统时间  
# 参数：一个形如 "YYYY-MM-DD HH:MM:SS" 的字符串  
set_system_time() {  
    # 检查参数是否为空  
    if [ -z "$1" ]; then  
        echo "Error: No date time string provided."  
        return 1  
    fi  

    # 使用date命令的-s选项来设置系统时间  
    # 注意：这需要root权限或以sudo方式运行  
    if sudo date -s "$1"; then  
        echo "System time set successfully to: $1"  
    else  
        echo "Failed to set system time. Please check your permissions."  
    fi  
}  

# 定义一个函数来设置目标主机的时间和时区  
function set_target_host_time_and_timezone {  
    local target_host=$1  
    local target_user=$2  
    local target_password=$3  
    local target_time=$4  
    local target_timezone=$5  
  
    # 使用sshpass通过SSH在目标主机上设置系统时间  
    sshpass -p "$target_password" ssh -o StrictHostKeyChecking=no $target_user@$target_host "sudo date -s '$target_time'"  
  
    # 设置目标主机的时区  
    sshpass -p "$target_password" ssh -o StrictHostKeyChecking=no $target_user@$target_host "sudo timedatectl set-timezone $target_timezone"  
  
    # 将系统时间写入硬件时钟  
    sshpass -p "$target_password" ssh -o StrictHostKeyChecking=no $target_user@$target_host "sudo hwclock --systohc"  
  
    echo "目标主机 $target_host 的时间和时区设置完成。"  
}  
  
# 获取当前主机的时间  
CURRENT_TIME=$(date '+%Y-%m-%d %H:%M:%S')  
  
# 获取当前主机的时区（这取决于你的系统和配置）  
CURRENT_TIMEZONE=$(timedatectl | grep 'Time zone' | cut -d: -f2 | xargs)  
  
# 目标主机的IP地址和用户名（这里假设用户名是root）  
TARGET_HOST="192.168.1.10"  
TARGET_USER="work"  
TARGET_PASSWORD="abc.1234"  # 硬编码的密码，非常不安全！  
  
  
# 调用函数设置目标主机的时间和时区  
set_target_host_time_and_timezone "$TARGET_HOST" "$CURRENT_TIME" "$CURRENT_TIMEZONE"

  
# set_system_time "2024-07-24 01:46:53"
# sudo timedatectl set-timezone Asia/Shanghai
# sudo hwclock --systohc
  
# 示例用法  
# 注意：你可能需要以root用户或使用sudo来运行这个脚本  

# sudo ./myapp configure1.json listen ens33 res.json