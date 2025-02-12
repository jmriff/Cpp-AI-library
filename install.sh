sudo apt update
sudo apt install cmake

mkdir build
cd build
cmake ..
make
mv libAI.so AI.so # Rename the .so file
sudo make install
cd ..
