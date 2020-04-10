cmake_minimum_required (VERSION 2.8.11)
project(trees)

include_directories(include/)
add_executable(trees src/main.cpp src/exceptions.cpp)