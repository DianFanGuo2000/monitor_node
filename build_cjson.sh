# ����cJSON���Դ����  
wget https://github.com/DaveGamble/cJSON/archive/refs/heads/master.zip  
  
# ��ѹ���ص�zip�ļ�  
unzip master.zip  
  
# �����ѹ���cJSONĿ¼  
cd cJSON-master  
  
# ����һ�����ڹ�����Ŀ¼  
mkdir build  
  
# ���빹��Ŀ¼  
cd build  
  
# ʹ��cmake������Ŀ  
cmake ..  
  
# ������Ŀ  
make  
  
# ��װ����õĿ��ͷ�ļ�  
sudo make install  
  
# ��������¶�̬�����������ļ����Ա�ϵͳ�����ҵ��°�װ�Ŀ�  
sudo touch /etc/ld.so.conf.d/libcjson.conf  
sudo echo "/usr/local/lib" | sudo tee -a /etc/ld.so.conf.d/libcjson.conf  
  
# ����ldconfig�Ը���ϵͳ�Ŀ⻺��  
sudo /sbin/ldconfig  
  
# ע�⣺��ʱcJSON��Ӧ���Ѿ���װ��ɣ���������������Ŀ��ͨ��#include <cjson/cJSON.h>��ʹ��