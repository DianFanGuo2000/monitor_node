


sudo ./myapp 3 ./VM-fpu/node_ifs.xml ./VM-fpu/links.xml ./VM-fpu/overall.json # ����xml�������õ�json������������
sudo ./myapp 1 VM ./VM-fpu/overall.json res.json # ����VM���Ի��ڵ���շ�



sudo ./myapp 3 ./fpu-fpu/node_ifs.xml ./fpu-fpu/links.xml ./fpu-fpu/overall.json # ����xml�������õ�json������������
sudo ./myapp 1 fpu-testee ./fpu-fpu/overall.json res.json # ����fpu���Ի��ڵ���շ�


sudo ./myapp 3 ./fpu-fpu/node_ifs.xml ./fpu-fpu/links.xml ./fpu-fpu/overall.json # ����xml�������õ�json������������
sudo ./myapp 1 fpu-cotestee ./fpu-fpu/overall.json res.json # ����fpu���Ի��ڵ���շ�


sudo ./myapp 2 fpu-testee ./fpu-fpu/overall.json ./fpu-fpu/fpu-testee.json # ��������ת���ɷ�����
sudo ./myapp 2 fpu-cotestee ./fpu-fpu/overall.json ./fpu-fpu/fpu-cotestee.json # ��������ת���ɷ�����


sudo ./myapp 0 gpu1.json res.json # ��json��������������

sudo bash reset_snmpd.sh \
/home/work/monitor_nodeV8.9/work_station/snmp_work_station/snmpd_bash/get_data.sh \
/home/work/monitor_nodeV8.9/work_station/snmp_work_station/snmpd_bash/bash_base/get_status.sh \
/home/work/monitor_nodeV8.9/work_station/snmp_work_station/snmpd_bash/bash_base/get_result.sh \
/home/work/monitor_nodeV8.9/work_station/snmp_work_station/oid_configs.json \
/home/work/monitor_nodeV8.9/work_station/snmp_work_station/snmpd.conf \
/etc/snmp/snmpd.conf \
/home/work/monitor_nodeV8.9/work_station/res.json

 # snmpd data parser bash file path
 # oid config json file path
 # the original snmpd.conf file path
 # current snmpd conf file with snmpd service
 # the path of result file from monitor node application
