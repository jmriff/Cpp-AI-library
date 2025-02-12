C++ AI Library

This library help create simple AIs with many different activations, and train them. After some testing, I found this library is faster than a C version of the same
library.

*The AI.hpp and AI.cpp files are buggy (The reason it is nn.run_round(...) instead of ai.train(...)), and not working at the moment, if you see the issue, I would be glad if you could send me the fixed files


Supported activations:

tanh        (Hyperbolic tangent)
relu        (Rectified linear unit)
sigmoid
fsigmoid    (Fast sigmoid)


Speed and accuracy (Single-thread on my machine with 2, 5, 1 network all tanh activations on XOR dataset):

10,000      epochs  |  speed: 00.0280812 seconds | score: 0.363387
100,000     epochs  |  speed: 00.2802770 seconds | score: 0.338100
1,000,000   epochs  |  speed: 02.5872500 seconds | score: 0.399083
10,000,000  epochs  |  speed: 24.8647000 seconds | score: 0.376373


Compile the code (example.cpp):

g++ example.cpp *.cpp -o example
