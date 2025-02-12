#!/bin/bash

clear

# Update package list and install cmake
echo "Updating package list and installing CMake..."
sudo apt update
sudo apt install -y cmake
echo "Done"

clear

# Create and navigate to the build directory
echo "Creating build directory..."
mkdir -p build
cd build
echo "Done"

# Run cmake, then make the files
echo "Installing..."
cmake ..
make
cd ..

# Install the files
sudo cp ./*.hpp /usr/local/include/
sudo cp build/libAI.so /usr/local/lib/
sudo ldconfig
echo "Done"

echo

echo "Installation complete!"
