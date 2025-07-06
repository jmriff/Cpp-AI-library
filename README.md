C++ AI Library

This library help create simple AIs with many different activations, and train them.

| Supported activations               |
|-------------------------------------|
| tanh        (Hyperbolic tangent)    |
| relu        (Rectified linear unit) |
| sigmoid                             |
| fsigmoid    (Fast sigmoid)          |

XOR dataset:

| Input A | Input B | Output |
|---------|---------|--------|
| 0       | 0       | 0      |
| 0       | 1       | 1      |
| 1       | 0       | 1      |
| 1       | 1       | 0      |

Installation
------------

```bash

# Install make and compiling tools
sudo apt install make gcc g++

# Compile and install the library
make
sudo make install

```

Usage
-----

Add -lAI to use it in your code. You can also run pre-made tests.

```bash
# Use it in your code
g++ yourcode.cpp -o yourcode -lAI

# Run test01
make test TEST=test01
