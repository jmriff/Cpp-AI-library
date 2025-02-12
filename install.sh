#!/bin/bash

clear

# Detect the OS
OS=$(uname)

# Install CMake for Linux or macOS
if [[ "$OS" == "Linux" ]]; then
    echo "Installing cmake for Linux..."
    # Attempting to install using apt (Debian/Ubuntu)
    if command -v apt &> /dev/null; then
        echo "Using apt to install CMake..."
        sudo apt update
        sudo apt install -y cmake
    # Attempting to install using yum (RHEL/CentOS)
    elif command -v yum &> /dev/null; then
        echo "Using yum to install CMake..."
        sudo yum install -y cmake
    # Attempting to install using dnf (Fedora)
    elif command -v dnf &> /dev/null; then
        echo "Using dnf to install CMake..."
        sudo dnf install -y cmake
    # Attempting to install using zypper (openSUSE)
    elif command -v zypper &> /dev/null; then
        echo "Using zypper to install CMake..."
        sudo zypper install -y cmake
    else
        echo "Package manager not detected. Please install CMake manually."
        exit 1
    fi
elif [[ "$OS" == "Darwin" ]]; then
    echo "Installing cmake for Mac OS..."
    # Check if brew is installed
    if command -v brew &> /dev/null; then
        echo "Using Homebrew to install CMake..."
        brew install cmake
    else
        echo "Homebrew not installed. Please install Homebrew first (https://brew.sh) and try again."
        exit 1
    fi
else
    echo "Unsupported OS detected. This script works only on Linux and macOS."
    echo "If using Windows, use the install.bat file"
    exit 1
fi

echo "Done."

# Create and navigate to the build directory
echo "Creating build directory..."
mkdir -p build
cd build
echo "Done."

# Run cmake, then make the files
echo "Building the project..."
cmake ..
make
cd ..

# Install the files
echo "Installing header files and library..."
sudo cp ./*.hpp /usr/local/include/
sudo cp build/libAI.so /usr/local/lib/
sudo ldconfig
echo "Done."

clear

echo "Installation complete!"
