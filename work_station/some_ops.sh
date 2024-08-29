


sudo ./myapp 3 ./VM-fpu/node_ifs.xml ./VM-fpu/links.xml ./VM-fpu/overall.json # 更新xml拓扑配置到json总拓扑配置中
sudo ./myapp 1 VM ./VM-fpu/overall.json res.json # 启动VM陪试机节点的收发



sudo ./myapp 3 ./fpu-fpu/node_ifs.xml ./fpu-fpu/links.xml ./fpu-fpu/overall.json # 更新xml拓扑配置到json总拓扑配置中
sudo ./myapp 1 fpu-testee ./fpu-fpu/overall.json res.json # 启动fpu被试机节点的收发


sudo ./myapp 3 ./fpu-fpu/node_ifs.xml ./fpu-fpu/links.xml ./fpu-fpu/overall.json # 更新xml拓扑配置到json总拓扑配置中
sudo ./myapp 1 fpu-cotestee ./fpu-fpu/overall.json res.json # 启动fpu陪试机节点的收发


sudo ./myapp 2 fpu-testee ./fpu-fpu/overall.json ./fpu-fpu/fpu-testee.json # 将总配置转换成分配置
sudo ./myapp 2 fpu-cotestee ./fpu-fpu/overall.json ./fpu-fpu/fpu-cotestee.json # 将总配置转换成分配置


sudo ./myapp 0 gpu1.json res.json # 从json分拓扑配置启动

