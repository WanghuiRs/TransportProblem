cmake_minimum_required(VERSION 3.13)
project(ProductionAndDistribution)

set(CMAKE_CXX_STANDARD 14)
include_directories(~/eigen3)
add_executable(ProductionAndDistribution main.cpp header/myGA.h header/data.h header/tools.h resource/GATwo.cpp resource/GATwo.h resource/Tool.cpp header/Tool.h)