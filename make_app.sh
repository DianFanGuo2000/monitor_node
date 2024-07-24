# ���ȣ�������Ϊ 'communication_monitor' ��Ŀ¼��ע��������ƴд��  
echo "Entering 'communication_monitor' directory..."  
cd communication_monitor  

# �ڸ�Ŀ¼��ִ�� make �����ͨ�������Դ���벢���ɿ�ִ���ļ������� Makefile �Ѿ����úã�  
echo "Executing 'make' command to compile sources..."  
make all
  
# �����ɵĿ�ִ���ļ�������Ϊ myapp�����Ƶ���һ��Ŀ¼  
echo "Copying 'myapp' executable to the parent directory..."  
cp myapp ../myapp  
  
# ���ţ��ڸ�Ŀ¼��ִ�� make clean ���������������������ɵ��ļ�  
echo "Executing 'make clean' command to clean up build artifacts..."  
make clean  
  
# ����˳���ǰĿ¼�����ص���һ��Ŀ¼  
echo "Exiting 'communication_monitor' directory and returning to the parent directory..."  
cd ..

