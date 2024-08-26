# 下载libxml库的源代码  
wget https://gitlab.gnome.org/GNOME/libxml2/-/archive/v2.10.1/libxml2-v2.10.1.tar.gz   # 下载官网：https://gitlab.gnome.org/GNOME/libxml2/-/releases

tar zxvf libxml2-v2.10.1.tar.gz

cd libxml2-v2.10.1/
mkdir release
export ACLOCAL_PATH=/usr/share/aclocal
./autogen.sh --with-http=no --with-python=no --with-lzma=no #如果有报错，先安装libtool，sudo apt-get install libtool

./configure --prefix=/usr/local/libxml2 # 配置

make clean
make
sudo make install

sudo ln -s /usr/include/libxml2/libxml   /usr/include/libxml