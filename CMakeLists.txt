cmake_minimum_required(VERSION 3.16)
project(dll_injector)

set(CMAKE_CXX_STANDARD 14)

add_executable(dll_injector src/main.cpp src/Processes/Processes.cpp src/Processes/Processes.h src/Process/Process.cpp src/Process/Process.h)