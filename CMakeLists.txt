cmake_minimum_required(VERSION 3.0.0)
project(kd_tree)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -o3")

add_executable(main main.cpp KDTree.cpp)
