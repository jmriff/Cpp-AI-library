C++ AI Library

This library help create simple AIs with many different activations, and train them. After some testing, I found this library is faster than a C version of the same
library.

* The AI.hpp and AI.cpp files are buggy, and not working at the moment, if you see the issue, I would be glad if someone could send me the fixed files

| Supported activations               |
|-------------------------------------|
| tanh        (Hyperbolic tangent)    |
| relu        (Rectified linear unit) |
| sigmoid                             |
| fsigmoid    (Fast sigmoid)          |

For this test, I am using the XOR dataset (down below) and a intel i3-12th generation processor, with the example.cpp file. The lower the score the better (the score is really just the error of the network)
| Epochs        | Speed (seconds)               | Score    |
|---------------|-------------------------------|----------|
| 10,000        | 00.0280812                    | 0.363387 |
| 100,000       | 00.2802770                    | 0.338100 |
| 1,000,000     | 02.5872500                    | 0.399083 |
| 10,000,000    | 24.8647000                    | 0.376373 |

XOR dataset:

| Input A | Input B | Output |
|---------|---------|--------|
| 0       | 0       | 0      |
| 0       | 1       | 1      |
| 1       | 0       | 1      |
| 1       | 1       | 0      |

Installation
------------

To install library, just run the install.sh script or Linux, and the install.bat script for Windows, and it will compile the file and install it on to your system, so when you use it, just add -lAI as a compiler flag.

Example compilation:
g++ example.cpp -o example -lAI

To run the example after compiling:
./example
