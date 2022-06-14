cmake_minimum_required(VERSION 3.21)
project(Exercise_3)

set(CMAKE_CXX_STANDARD 11)

add_executable(Exercise_3 main.cpp Controller.cpp Geometry.cpp Model.cpp Vehicle.cpp View.cpp Warehouse.cpp)
