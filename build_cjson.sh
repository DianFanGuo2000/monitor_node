# 下载cJSON库的源代码  
wget https://github.com/DaveGamble/cJSON/archive/refs/heads/master.zip  
  
# 解压下载的zip文件  
unzip master.zip  
  
# 进入解压后的cJSON目录  
cd cJSON-master  
  
# 创建一个用于构建的目录  
mkdir build  
  
# 进入构建目录  
cd build  
  
# 使用cmake配置项目  
cmake ..  
  
# 编译项目  
make  
  
# 安装编译好的库和头文件  
sudo make install  
  
# 创建或更新动态链接器配置文件，以便系统可以找到新安装的库  
sudo touch /etc/ld.so.conf.d/libcjson.conf  
sudo echo "/usr/local/lib" | sudo tee -a /etc/ld.so.conf.d/libcjson.conf  
  
# 运行ldconfig以更新系统的库缓存  
sudo /sbin/ldconfig  
  
# 注意：此时cJSON库应该已经安装完成，并可以在您的项目中通过#include <cjson/cJSON.h>来使用