# From the /root/mirror_project folder:
rm -rf build
mkdir build
cd build
cmake ..
make -j4
sudo ./mirror_app
