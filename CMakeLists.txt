cmake_minimum_required(VERSION 3.16)
project(Containers C)

set(CMAKE_C_STANDARD 11)

include_directories(include)

add_executable(Containers main.c include/buffer.h include/buffer.c)