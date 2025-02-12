cmake_minimum_required(VERSION 3.10)
project(Cpp-AI-library VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(AI SHARED activations.cpp nn.cpp AI.cpp)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-type-limits")
