# ����libxml���Դ����  
wget https://gitlab.gnome.org/GNOME/libxml2/-/archive/v2.10.1/libxml2-v2.10.1.tar.gz   # ���ع�����https://gitlab.gnome.org/GNOME/libxml2/-/releases

tar zxvf libxml2-v2.10.1.tar.gz

cd libxml2-v2.10.1/
mkdir release
export ACLOCAL_PATH=/usr/share/aclocal
./autogen.sh --with-http=no --with-python=no --with-lzma=no #����б����Ȱ�װlibtool��sudo apt-get install libtool

./configure --prefix=/usr/local/libxml2 # ����

make clean
make
sudo make install

sudo ln -s /usr/include/libxml2/libxml   /usr/include/libxml